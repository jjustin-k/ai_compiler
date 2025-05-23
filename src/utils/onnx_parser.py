import onnx

model = onnx.load("./models/resnet50_Opset18.onnx")

onnx.checker.check_model(model)

print(onnx.helper.printable_graph(model.graph))