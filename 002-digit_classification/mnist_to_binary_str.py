from torchvision import datasets, transforms
import sys

# --- データセットの準備（ToTensorで[0,1]に正規化） ---
mnist = datasets.MNIST(
    root="./data",
    train=True,
    download=True,
    transform=transforms.ToTensor()
)

to_bin = lambda x: (x > 0.5).numpy().astype(int)

wanted_label = int(sys.argv[1]) if len(sys.argv) > 1 else 0

for img, label in mnist:
    if label == wanted_label:
        # img shape: (1, 28, 28)
        bin_img = to_bin(img.squeeze())  # shape: (28, 28)

        for row in bin_img:
            print("".join(str(x) for x in row))
        break
