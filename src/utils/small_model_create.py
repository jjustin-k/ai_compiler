import onnx
import onnx.helper as helper
import onnx.numpy_helper as numpy_helper
import numpy as np

# Input tensor
input_tensor = helper.make_tensor_value_info("input", onnx.TensorProto.FLOAT, [1, 1, 5, 5])

# Output tensor
output_tensor = helper.make_tensor_value_info("output", onnx.TensorProto.FLOAT, [1, 1, 3, 3])

# Weights: 1 filter of shape (1, 1, 3, 3)
weights = np.ones((1, 1, 3, 3)).astype(np.float32)
weight_initializer = numpy_helper.from_array(weights, name="W")

# Bias: 1 value
bias = np.array([0.0]).astype(np.float32)
bias_initializer = numpy_helper.from_array(bias, name="B")

# Conv node
conv_node = helper.make_node(
    "Conv",
    inputs=["input", "W", "B"],
    outputs=["output"],
    kernel_shape=[3, 3],
    strides=[1, 1],
    pads=[0, 0, 0, 0]
)

# Graph
graph_def = helper.make_graph(
    nodes=[conv_node],
    name="TinyConvNet",
    inputs=[input_tensor],
    outputs=[output_tensor],
    initializer=[weight_initializer, bias_initializer]
)

# Model
model_def = helper.make_model(graph_def, producer_name="custom-onnx")
onnx.save(model_def, "models/tiny_conv.onnx")
