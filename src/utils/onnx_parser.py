import onnx
import json
import numpy as np
from onnx import numpy_helper

def tensor_to_list(tensor):
    return numpy_helper.to_array(tensor).tolist()

def save_onnx_to_json(onnx_path, json_path):
    model = onnx.load(onnx_path)
    graph = model.graph

    def make_serializable(value):
        # Convert protobuf containers to lists/dicts
        if isinstance(value, onnx.TensorProto):
            return tensor_to_list(value)
        if isinstance(value, onnx.AttributeProto):
            return {value.name: onnx.helper.get_attribute_value(value)}
        if isinstance(value, (list, tuple)):
            return [make_serializable(v) for v in value]
        return value

    model_json = {
        "inputs": [{"name": inp.name, "type": str(inp.type)} for inp in graph.input],
        "outputs": [{"name": out.name, "type": str(out.type)} for out in graph.output],
        "nodes": [],
        "initializers": {}
    }

    for tensor in graph.initializer:
        model_json["initializers"][tensor.name] = {
            "dims": list(tensor.dims),
            "data_type": tensor.data_type,
            "values": tensor_to_list(tensor)
        }

    for node in graph.node:
        model_json["nodes"].append({
            "name": node.name,
            "op_type": node.op_type,
            "inputs": list(node.input),
            "outputs": list(node.output),
            "attributes": [make_serializable(attr) for attr in node.attribute]
        })

    with open(json_path, "w") as f:
        json.dump(model_json, f, indent=2)

# Example usage
save_onnx_to_json("ai_compiler/models/tiny_conv.onnx", "models/tiny_cov.json")
