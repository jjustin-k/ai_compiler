import onnx
import json
from onnx import numpy_helper

def tensor_to_list(tensor):
    return numpy_helper.to_array(tensor).tolist()

def make_serializable(value):
    if isinstance(value, onnx.TensorProto):
        return tensor_to_list(value)
    if isinstance(value, (list, tuple)):
        return [make_serializable(v) for v in value]
    return value

def attributes_to_dict(attrs):
    result = {}
    for attr in attrs:
        val = onnx.helper.get_attribute_value(attr)
        # Decode bytes to string if needed
        if isinstance(val, bytes):
            val = val.decode('utf-8')
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
save_onnx_to_json("models/mnist-12-int8.onnx", "models/mnist-12-int8.json")
