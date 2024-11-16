from PIL import Image

# Load the image
image = Image.open("wood_cp.png")

# Resize the image to half its size (32x32)
resized_image = image.resize((32, 32), Image.ANTIALIAS)

# Save the resized image
resized_image.save("resized_wood.png")