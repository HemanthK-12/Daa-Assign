import matplotlib.pyplot as plt
import numpy as np

# Data for the bars
labels = ['h=2', 'h=3', 'h=4']
values = [15.5, 155, 1123.75]
plt.figure(figsize=(10, 6))
bars = plt.bar(labels, values)

# Add value labels on top of each bar
for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height,
             f'{height}',
             ha='center', va='bottom')

# Customize the plot
plt.title('Densities of Densest Subgraphs on CA-HepTh Dataset')
plt.xlabel('Clique Instance')
plt.ylabel('Density')
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Show the plot
plt.show()

