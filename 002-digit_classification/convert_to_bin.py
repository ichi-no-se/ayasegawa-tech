import torch
import json
from model import MLP
import numpy as np

model = MLP()
model.load_state_dict(torch.load("best_model_gray.pth", weights_only=True))
model.eval()

weights =[]
shapes = {}

for key, tensor in model.state_dict().items():
    array = tensor.numpy().astype(np.float32)
    weights.append(array.flatten())
    name,attr = key.split(".")
    if name not in shapes:
        shapes[name] = {}
    shapes[name][attr] = list(array.shape)

concatenated_weights = np.concatenate(weights)
concatenated_weights.tofile("model_weights.bin")



with open("model_shapes.json", "w") as f:
    json.dump(shapes, f)

