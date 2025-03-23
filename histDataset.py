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
        bars=plt.bar(indices + i * bar_width, values, width=bar_width, label=alg)
        for idx, rect in enumerate(bars):
            height = rect.get_height()
            plt.text(rect.get_x() + rect.get_width()/2., height,
                f'{values[idx]:.7f}s',
                ha='center', va='bottom')
    plt.xlabel("Datasets")
    plt.ylabel("Time (seconds)")
    plt.title("Execution Time for Algorithms on Different Datasets")
    plt.xticks(indices + bar_width, datasets)
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    plt.show()

# Example usage
time_values = {
    # ("1-clique-worst-case", "Enron Mail Network"): 1.83627,
    # ("2-bron-kerbosch-degeneracy", "Enron Mail Network"): 1.5,
    # ("3-clique-arboricity", "Enron Mail Network"): 1.1,
    # ("1-clique-worst-case", "Wikipedia Vote System"): 1.81468,
    # ("2-bron-kerbosch-degeneracy", "Wikipedia Vote System"): 2.1,
    # ("3-clique-arboricity", "Wikipedia Vote System"): 2.4,
    ("1-clique-worst-case", "Autonomous systems for Skitter"): 379.6,
    # ("2-bron-kerbosch-degeneracy", "Autonomous systems for Skitter"): 2.9,
    # ("3-clique-arboricity", "Autonomous systems for Skitter"): 3.2
}

plot_histogram(time_values)

