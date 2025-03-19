import matplotlib.pyplot as plt
import numpy as np

def plot_histogram(times):
    """
    Plots a grouped bar chart for the given time values, with more spacing between datasets.
    :param times: A dictionary with keys as tuples (algorithm, dataset) and values as time taken.
    """
    datasets = sorted(set(dataset for _, dataset in times.keys()))
    algorithms = sorted(set(alg for alg, _ in times.keys()))
    
    bar_width = 0.2  # Width of each bar
    spacing = 0.5  # Space between datasets
    indices = np.arange(len(datasets)) * (len(algorithms) * bar_width + spacing)
    
    plt.figure(figsize=(10, 6))
    
    for i, alg in enumerate(algorithms):
        values = [times[(alg, dataset)] for dataset in datasets]
        plt.bar(indices + i * bar_width, values, width=bar_width, label=alg)
    
    plt.xlabel("Datasets")
    plt.ylabel("Time (seconds)")
    plt.title("Execution Time for Algorithms on Different Datasets")
    plt.xticks(indices + bar_width, datasets)
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    plt.show()

# Example usage
time_values = {
    ("Alg1", "Dataset1"): 1.2,
    ("Alg2", "Dataset1"): 1.5,
    ("Alg3", "Dataset1"): 1.1,
    ("Alg1", "Dataset2"): 2.3,
    ("Alg2", "Dataset2"): 2.1,
    ("Alg3", "Dataset2"): 2.4,
    ("Alg1", "Dataset3"): 3.0,
    ("Alg2", "Dataset3"): 2.9,
    ("Alg3", "Dataset3"): 3.2
}

plot_histogram(time_values)

