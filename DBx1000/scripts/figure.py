import pandas as pd
import seaborn as sns
df = pd.read_csv("results.csv")
bar_plot = sns.catplot(x="WORKLOAD", y="throughput",
            hue="CC_ALG",
            data=df, kind='bar')
bar_plot.set(xlabel='Workload', ylabel="Throughput, txn/s")
bar_plot.savefig("bar_plot.pdf")
