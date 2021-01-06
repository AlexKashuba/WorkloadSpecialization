import pandas as pd
import json
import sys

header = False
if len(sys.argv) > 2 and sys.argv[2] == '-h':
    header=True
jobname = sys.argv[1]

with open("data/%s.res" % jobname, 'r') as f:
    data = json.load(f)

df = pd.DataFrame(data['data'])
df['experiment'] = jobname
df.drop(['debug1', 'debug2', 'debug3', 'debug4', 'debug5'], axis=1, inplace=True)
for k, v in data['conf'].items():
    df[k] = v

for row in df.iterrows():
    data = row[1]
    if data['CC_ALG'] == 'TIMESTAMP' and data['opt']:
        df.loc[row[0], 'CC_ALG'] = 'TIMESTAMP_OPT'

print(df.to_csv(header=header))
