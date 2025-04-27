from PIL import Image
import numpy as np


def image_to_matrix(image_path, grid_size):
    """Takes in a QR code image and grid size to output a binary matrix of the QR Code"""
    img = Image.open(image_path).convert("L")  # 'L' mode = grayscale

    # Resize to square
    min_side = min(img.size)
    img = img.crop((0, 0, min_side, min_side))

    # Resize image to match grid size
    img_resized = img.resize((grid_size, grid_size), Image.Resampling.LANCZOS)

    # Convert image to NumPy array
    img_array = np.array(img_resized)

    # Normalize to 0 (white) or 1 (black) based on brightness threshold
    threshold = 128
    matrix = (img_array < threshold).astype(int)

    return matrix.tolist()


def matrix_to_qr(matrix, scale=10, black=(0, 0, 0), white=(255, 255, 255)):
    """Takes in a binary matrix of a QR code and outputs a digital image"""
    size = len(matrix)
    img_size = size * scale

    img = Image.new("RGB", (img_size, img_size), color=white)
    pixels = img.load()

    for y in range(size):
        for x in range(size):
            color = black if matrix[y][x] == 1 else white
            for dy in range(scale):
                for dx in range(scale):
                    pixels[x * scale + dx, y * scale + dy] = color

    return img


# Example usage
if __name__ == "__main__":
    image_path = "example.png"  # Replace with your file path
    grid_size = 25

    matrix = image_to_matrix(image_path, grid_size)

    for row in matrix:
        print(row)

    img = matrix_to_qr(matrix, scale=20)
    img.show()
    img.save("example.png")
