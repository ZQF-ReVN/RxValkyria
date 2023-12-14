import argparse
import struct
import os

class Entry:
    def __init__(self, name=None, offset=None, size=None, file_buffer=None):
        self.name = name
        self.offset = offset
        self.size = size
        self.file_buffer = file_buffer

def unpack(dat_file, output_dir):
    with open(dat_file, 'rb') as f:
        dat_buffer = f.read()

    # Read the number of entries in the .dat file
    num_entries = struct.unpack('<I', dat_buffer[:4])[0]

    # Check if the number of entries is valid
    if num_entries == 0 or num_entries >= len(dat_buffer):
        return []

    # Calculate the number of entries and make sure it is a multiple of 268
    num_entries //= 268
    if num_entries * 268 != struct.unpack('<I', dat_buffer[:4])[0] or num_entries > 4000:
        return []

    directory = []

    # Iterate through each entry and extract its name, offset, and size
    for i in range(num_entries):
        # Extract the name of the entry
        name = dat_buffer[4 + i * 268: 4 + i * 268 + 260]
        name = name[:name.index(b'\x00')].decode('shift-jis')

        # Extract the offset and size of the entry
        offset, size = struct.unpack('<II', dat_buffer[4 + i * 268 + 260: 4 + i * 268 + 268])

        # Add the entry to the directory list
        directory.append(Entry(name=name, offset=offset, size=size))

    # Calculate the start of the data section
    data_start = 4 + num_entries * 268

    for entry in directory:
        start = data_start + entry.offset
        end = start + entry.size

        # Read the entry data from the .dat file
        entry.file_buffer = dat_buffer[start:end]

        output_path = os.path.join(output_dir, entry.name)

        if not os.path.exists(os.path.dirname(output_path)):
            os.makedirs(os.path.dirname(output_path))

        with open(output_path, 'wb') as f:
            f.write(entry.file_buffer)

def pack(input_dir, output_file):
    directory = []
    file_data = b''

    for root, dirs, files in os.walk(input_dir):
        for file in files:
            file_path = os.path.join(root, file)
            with open(file_path, 'rb') as f:
                file_buffer = f.read()

            entry = Entry(name=file, offset=len(file_data), size=len(file_buffer), file_buffer=file_buffer)

            directory.append(entry)
            file_data += file_buffer

    # Calculate the total size of the directory
    directory_size = len(directory) * 268

    header = struct.pack('<I', directory_size)

    for entry in directory:
        name = entry.name.encode('shift-jis')

        # Pad the entry name with null bytes to make it 260 bytes long
        name += b'\x00' * (260 - len(name))

        header += name + struct.pack('<II', entry.offset, entry.size)

    with open(output_file, 'wb') as f:
        f.write(header + file_data)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Unpack or pack Valkyria\'s .dat archive')
    parser.add_argument('mode', choices=['unpack', 'pack'], help='The mode to run the script in')
    parser.add_argument('input', help='The input file or directory')
    parser.add_argument('output', help='The output file or directory')
    args = parser.parse_args()

    if args.mode == 'unpack':
        unpack(args.input, args.output)
    elif args.mode == 'pack':
        pack(args.input, args.output)
