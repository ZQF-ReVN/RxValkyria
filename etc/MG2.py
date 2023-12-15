from PIL import Image
import struct
import io
import math

class Mico:
    @staticmethod
    def create(source_path, output_path):
        source = Image.open(source_path)

        if source.mode != 'RGB' and source.mode != 'RGBA':
            raise Exception("Only 24bpp and 32bpp images are supported.")

        # Stage 1: Flip
        source = source.transpose(Image.FLIP_TOP_BOTTOM)

        # Stage 2: Generate color image of source
        color_layer = source.convert("RGB")
        color_layer_stream = io.BytesIO()
        color_layer.save(color_layer_stream, format='PNG')
        color_layer_data = color_layer_stream.getvalue()

        # Stage 3: Generate alpha image of source
        alpha_layer_data = b''
        if source.mode == 'RGBA':
            alpha_layer_pixels = []

            for pixel in source.getdata():
                alpha_layer_pixels.extend([pixel[3], pixel[3], pixel[3]])

            alpha_layer_image = Image.frombytes('RGB', source.size, bytes(alpha_layer_pixels))

            alpha_layer_stream = io.BytesIO()
            alpha_layer_image.save(alpha_layer_stream, format='PNG')
            alpha_layer_data = alpha_layer_stream.getvalue()

        # Stage 4: Write
        with open(output_path, 'wb') as writer:
            writer.write(struct.pack('I', 0x4F43494D))  # MICO
            writer.write(struct.pack('I', 0x31304743))  # CG01
            writer.write(struct.pack('I', len(color_layer_data)))
            writer.write(struct.pack('I', len(alpha_layer_data)))

            if len(color_layer_data) > 0:
                color_layer_data = Mico.encrypt_image_data(color_layer_data)
                writer.write(color_layer_data)

            if len(alpha_layer_data) > 0:
                alpha_layer_data = Mico.encrypt_image_data(alpha_layer_data)
                writer.write(alpha_layer_data)

    @staticmethod
    def encrypt_image_data(data):
        threshold = min(25, len(data))
        key = len(data)

        data = bytearray(data)

        for i in range(threshold):
            data[i] ^= (key + i) % 256

        return bytes(data)


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 3:
        print("Usage:")
        print("mg2.py input.png output.mg2")
    else:
        Mico.create(sys.argv[1], sys.argv[2])
