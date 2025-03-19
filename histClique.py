import matplotlib.pyplot as plt
import numpy as np

def plot_clique_distribution(clique_sizes):
    """
    Plots a grouped bar chart for the distribution of different size cliques for each dataset.
    :param clique_sizes: A dictionary with keys as tuples (clique size, dataset) and values as count.
    """
    datasets = sorted(set(dataset for _, dataset in clique_sizes.keys()))
    clique_types = sorted(set(size for size, _ in clique_sizes.keys()))
    
    bar_width = 0.2  # Width of each bar
    spacing = 1.0  # Space between datasets
    indices = np.arange(len(datasets)) * (len(clique_types) * bar_width + spacing)
    
    plt.figure(figsize=(12, 7))
    
    for i, clique_size in enumerate(clique_types):
        values = [clique_sizes.get((clique_size, dataset), 0) for dataset in datasets]
        plt.bar(indices + i * bar_width, values, width=bar_width, label=f"Clique {clique_size}")
    
    plt.xlabel("Datasets")
    plt.ylabel("Number of Maximal Cliques")
    plt.title("Distribution of Different Size Cliques Across Datasets")
    plt.xticks(indices + (len(clique_types) * bar_width) / 2, datasets)
    plt.legend()
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    plt.show()

# Example usage
clique_data = {
    (2, "Enron Mail Network"): 8655,
    (3, "Enron Mail Network"): 13718,
    (4, "Enron Mail Network"): 27292,
    (5, "Enron Mail Network"): 48416,
    (6, "Enron Mail Network"): 68872,
    (7, "Enron Mail Network"): 83266,
    (8, "Enron Mail Network"): 76732,
    (9, "Enron Mail Network"): 54456,
    (10, "Enron Mail Network"): 35470,
    (11, "Enron Mail Network"): 21736,
    (12, "Enron Mail Network"): 11640,
    (13, "Enron Mail Network"): 5449,
    (14, "Enron Mail Network"): 2329,
    (15, "Enron Mail Network"): 740,
    (16, "Enron Mail Network"): 208,
    (17, "Enron Mail Network"): 23,
    (2, "Wikipedia Vote System"): 14070,
    (3, "Wikipedia Vote System"): 7077,
    (4, "Wikipedia Vote System"): 13319,
    (5, "Wikipedia Vote System"): 18143,
    (6, "Wikipedia Vote System"): 22715,
    (7, "Wikipedia Vote System"): 25896,
    (8, "Wikipedia Vote System"): 24766,
    (9, "Wikipedia Vote System"): 22884,
    (10, "Wikipedia Vote System"): 21393,
    (11, "Wikipedia Vote System"): 17833,
    (12, "Wikipedia Vote System"): 15181,
    (13, "Wikipedia Vote System"): 11487,
    (14, "Wikipedia Vote System"): 7417,
    (15, "Wikipedia Vote System"): 3157,
    (16, "Wikipedia Vote System"): 1178,
    (17, "Wikipedia Vote System"): 286,
    (18, "Wikipedia Vote System"): 41,
    (19, "Wikipedia Vote System"): 10,
    (20, "Wikipedia Vote System"): 6,
    (3, "Skitter"): 80,
    (4, "Skitter"): 50,
    (5, "Skitter"): 20
}

plot_clique_distribution(clique_data)

