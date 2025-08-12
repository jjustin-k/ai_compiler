import onnx
import json
from onnx import numpy_helper
from google.protobuf.message import Message

def tensor_to_list(tensor):
    return numpy_helper.to_array(tensor).tolist()

def make_serializable(value):
    # Recursively convert ONNX protobuf objects to native Python types
    if isinstance(value, onnx.TensorProto):
        array = numpy_helper.to_array(value)
        return {
            "dims": list(value.dims),
            "data_type": onnx.TensorProto.DataType.Name(value.data_type),
            "values": array.tolist(),
            "shape": list(array.shape)
        }
    if isinstance(value, Message):
        # Convert protobuf message to dict recursively
        result = {}
        for field, val in value.ListFields():
            result[field.name] = make_serializable(val)
        return result
    if isinstance(value, (list, tuple)):
        return [make_serializable(v) for v in value]
    if isinstance(value, bytes):
        try:
            return value.decode('utf-8')
        except Exception:
            return list(value)  # fallback to list of ints
    return value

def attributes_to_dict(attrs):
    result = {}
    for attr in attrs:
        val = onnx.helper.get_attribute_value(attr)
        val = make_serializable(val)
        result[attr.name] = val
    return result

def save_onnx_to_json(onnx_path, json_path):
    model = onnx.load(onnx_path)
    graph = model.graph

    def parse_value_info(value_info):
        t = value_info.type.tensor_type
        return {
            "name": value_info.name,
            "elem_type": onnx.TensorProto.DataType.Name(t.elem_type),
            "shape": [d.dim_value for d in t.shape.dim]
        }

    model_json = {
        "inputs": [parse_value_info(inp) for inp in graph.input],
        "outputs": [parse_value_info(out) for out in graph.output],
        "nodes": [],
        "initializers": {}
    }

    for tensor in graph.initializer:
        model_json["initializers"][tensor.name] = {
            "dims": list(tensor.dims),
            "data_type": onnx.TensorProto.DataType.Name(tensor.data_type),
            "values": tensor_to_list(tensor)
        }

    for node in graph.node:
        model_json["nodes"].append({
            "name": node.name,
            "op_type": node.op_type,
            "inputs": list(node.input),
            "outputs": list(node.output),
            "attributes": attributes_to_dict(node.attribute)
        })

    with open(json_path, "w") as f:
        json.dump(model_json, f, indent=2)

# Example usage:
save_onnx_to_json("models/mnist-8.onnx", "models/mnist-8.json")
