import sys
import json

SUMMARY_MARKER = "[summary]"

def parse_output(s):
    summary_idx = s.find(SUMMARY_MARKER)
    data_str = s[summary_idx + len(SUMMARY_MARKER):].strip()
    data = {name.strip(): float(val) 
                for [name,val] in [
                    pair.split('=') for pair in data_str.split(',')
                ]
           }
    data['throughput'] = data['txn_cnt'] / data['run_time']
    return data

if __name__ == "__main__":
    sep = ''
    if len(sys.argv) > 1 and sys.argv[1] == '-s':
        sep = ','
    input_str = sys.stdin.read() 
    print(json.dumps(parse_output(input_str)) + sep + '\n')
