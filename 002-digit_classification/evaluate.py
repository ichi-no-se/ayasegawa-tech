from model import MLP
import torch

lines = []
for i in range(28):
    lines.append(input())
input_tensor = torch.tensor([[int(c) for c in s] for s in lines], dtype=torch.float32).view(1, 1, 28, 28)

model_path = 'best_model.pth'
model = MLP()
model.load_state_dict(torch.load(model_path, weights_only=True))
model.eval()

with torch.no_grad():
    logits = model(input_tensor)
    probs = torch.softmax(logits, dim=1)
    for i in range(10):
        print(f"Probability of {i}: {probs[0][i].item():.4f}")
