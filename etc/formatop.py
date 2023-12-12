import json

def read_op_list():
    with open('op.txt', 'r', encoding='utf-8') as ifs:
        return ifs.readlines()


def format_str(op_str_list):
    op_list = {}
    for op_str in op_str_list:
        split_list = op_str.split(':')
        opcode = 0
        if '0x' in split_list[1]:
            opcode = int(split_list[1], 16)
        else:
            opcode = int(split_list[1])
        split_list[1] = '0x%08x' % opcode
        op_list[opcode] = split_list[2]
    return op_list



def out_op_list(op_list):
    op_json = []
    for k, v in op_list.items():
        op_json.append(
            {
                'OP': '0x%04X' % k,
                'Name': v.strip().replace('"','')
            }
        )
    with open('op1.json', 'w', encoding='utf-8') as ofs:
        json.dump(op_json, ofs, ensure_ascii=False, indent=4)

test_list = []
test_list.append('sfsdf\tffs\r');
with open('xx.json', 'w', encoding='utf-8') as ofs:
    json.dump(test_list, ofs, ensure_ascii=False, indent=4)
# format_str()
# out_op_list()
# print()
