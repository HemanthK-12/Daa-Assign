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
    # (2, "Enron Mail Network"): 8655,
    # (3, "Enron Mail Network"): 13718,
    # (4, "Enron Mail Network"): 27292,
    # (5, "Enron Mail Network"): 48416,
    # (6, "Enron Mail Network"): 68872,
    # (7, "Enron Mail Network"): 83266,
    # (8, "Enron Mail Network"): 76732,
    # (9, "Enron Mail Network"): 54456,
    # (10, "Enron Mail Network"): 35470,
    # (11, "Enron Mail Network"): 21736,
    # (12, "Enron Mail Network"): 11640,
    # (13, "Enron Mail Network"): 5449,
    # (14, "Enron Mail Network"): 2329,
    # (15, "Enron Mail Network"): 740,
    # (16, "Enron Mail Network"): 208,
    # (17, "Enron Mail Network"): 23,
    # (2, "Wikipedia Vote System"): 14070,
    # (3, "Wikipedia Vote System"): 7077,
    # (4, "Wikipedia Vote System"): 13319,
    # (5, "Wikipedia Vote System"): 18143,
    # (6, "Wikipedia Vote System"): 22715,
    # (7, "Wikipedia Vote System"): 25896,
    # (8, "Wikipedia Vote System"): 24766,
    # (9, "Wikipedia Vote System"): 22884,
    # (10, "Wikipedia Vote System"): 21393,
    # (11, "Wikipedia Vote System"): 17833,
    # (12, "Wikipedia Vote System"): 15181,
    # (13, "Wikipedia Vote System"): 11487,
    # (14, "Wikipedia Vote System"): 7417,
    # (15, "Wikipedia Vote System"): 3157,
    # (16, "Wikipedia Vote System"): 1178,
    # (17, "Wikipedia Vote System"): 286,
    # (18, "Wikipedia Vote System"): 41,
    # (19, "Wikipedia Vote System"): 10,
    # (20, "Wikipedia Vote System"): 6
    (2, "Skitter"): 2319807,
    (3, "Skitter"): 3171609,
    (4, "Skitter"): 1823321,
    (5, "Skitter"): 939336,
    (6, "Skitter"): 684873,
    (7, "Skitter"): 598284,
    (8, "Skitter"): 588889,
    (9, "Skitter"): 608937,
    (10, "Skitter"): 665661,
    (11, "Skitter"): 728098,
    (12, "Skitter"): 798073,
    (13, "Skitter"): 877282,
    (14, "Skitter"): 945194,
    (15, "Skitter"): 980831,
    (16, "Skitter"): 939987,
    (17, "Skitter"): 839330,
    (18, "Skitter"): 729601,
    (19, "Skitter"): 639413,
    (20, "Skitter"): 600192,
    (21, "Skitter"): 611976,
    (22, "Skitter"): 640890,
    (23, "Skitter"): 673924,
    (24, "Skitter"): 706753,
    (25, "Skitter"): 753633,
    (26, "Skitter"): 818353,
    (27, "Skitter"): 892719,
    (28, "Skitter"): 955212,
    (29, "Skitter"): 999860,
    (30, "Skitter"): 1034106,
    (31, "Skitter"): 1055653,
    (32, "Skitter"): 1017560,
    (33, "Skitter"): 946717,
    (34, "Skitter"): 878552,
    (35, "Skitter"): 809485,
    (36, "Skitter"): 744634,
    (37, "Skitter"): 663650,
    (38, "Skitter"): 583922,
    (39, "Skitter"): 520239,
    (40, "Skitter"): 474301,
    (41, "Skitter"): 420796,
    (42, "Skitter"): 367879,
    (43, "Skitter"): 321829,
    (44, "Skitter"): 275995,
    (45, "Skitter"): 222461,
    (46, "Skitter"): 158352,
    (47, "Skitter"): 99522,
    (48, "Skitter"): 62437,
    (49, "Skitter"): 39822,
    (50, "Skitter"): 30011,
    (51, "Skitter"): 25637,
    (52, "Skitter"): 17707,
    (53, "Skitter"): 9514,
    (54, "Skitter"): 3737,
    (55, "Skitter"): 2042,
    (56, "Skitter"): 1080,
    (57, "Skitter"): 546,
    (58, "Skitter"): 449,
    (59, "Skitter"): 447,
    (60, "Skitter"): 405,
    (61, "Skitter"): 283,
    (62, "Skitter"): 242,
    (63, "Skitter"): 146,
    (64, "Skitter"): 84,
    (65, "Skitter"): 49,
    (66, "Skitter"): 22,
    (67, "Skitter"): 4
}

plot_clique_distribution(clique_data)

