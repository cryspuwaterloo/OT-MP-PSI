""" ONLY FOR NON-PAPER USAGE (IN PAPER WE WANT TO HAVE A TABLE) """

import matplotlib.pyplot as plt

import numpy as np

# Set base-path to the folder with all the extracted json files from the reconstruction
import json
import os
from Graphs import paths

base_path = paths.SHAREGEN_PATH
datacenter_path = os.path.join(base_path, "sharegen_samedatacenter")
continent_path = os.path.join(base_path, "sharegen_acrosscontinent")
atlantic_path = os.path.join(base_path, "sharegen_acrossatlantic")


def get_result_fp(filepath):
    """ Load all json files """
    result_jsons = []
    for root, dirs, files in os.walk(filepath):
        for file in files:
            if ".json" in file:
                result_jsons.append(os.path.join(root, file))
    return result_jsons


def load_matrix_s1(filepaths):
    """ Loads matrices with (1) average time, (2) total comm and (3) standard deviations
        for both scheme 1
    """
    t_list, avg_list, comm_list, std_list = [], [], [], []
    for file in filepaths:
        with open(file) as json_file:
            data = json.load(json_file)
            scheme = data["scheme"]
            if scheme != 1:
                continue
            t, avg, comm, std = data["t"], data["s1time_avg"], data["s1comm_total"], data["s1time_std"]
            t_list.append(t)
            avg_list.append(avg)
            comm_list.append(comm)
            std_list.append(std)

    # Sort with t
    sort_idx = np.argsort(t_list)
    t_list = np.array(t_list)[sort_idx]
    comm_list = np.array(comm_list)[sort_idx]
    avg_list = np.array(avg_list)[sort_idx]
    std_list = np.array(std_list)[sort_idx]

    return {"t":t_list, "avg": avg_list, "comm": comm_list, "std":std_list}

def load_matrix_s0(filepaths):
    """ Loads matrices with (1) average time, (2) total comm and (3) standard deviations
        for both scheme 1
    """
    t_list, avg_list, comm_list, std_list = [], [], [], []
    for file in filepaths:
        with open(file) as json_file:
            data = json.load(json_file)
            scheme = data["scheme"]
            if scheme != 2:
                continue
            print(data)
            t, avg, comm, std = data["t"], data["s0time_avg"], data["s0comm_total"], data["s0time_std"]
            t_list.append(t)
            avg_list.append(avg)
            comm_list.append(comm)
            std_list.append(std)

    # Sort with t
    sort_idx = np.argsort(t_list)
    t_list = np.array(t_list)[sort_idx]
    comm_list = np.array(comm_list)[sort_idx]
    avg_list = np.array(avg_list)[sort_idx]
    std_list = np.array(std_list)[sort_idx]

    return {"t":t_list, "avg": avg_list, "comm": comm_list, "std":std_list}


dc_result_s1 = load_matrix_s1(get_result_fp(datacenter_path))
ct_result_s1 = load_matrix_s1(get_result_fp(continent_path))
ac_result_s1 = load_matrix_s1(get_result_fp(atlantic_path))

dc_result_s0 = load_matrix_s0(get_result_fp(datacenter_path))
ct_result_s0 = load_matrix_s0(get_result_fp(continent_path))
ac_result_s0 = load_matrix_s0(get_result_fp(atlantic_path))


# Plot Generation Time for S1
plt.title("Share Generation Time - Scheme 2")
plt.xlabel("Threshold t")
plt.ylabel("Time in Milliseconds")
plt.grid()
plt.errorbar(dc_result_s1["t"], dc_result_s1["avg"], label="Same Datacenter", yerr=dc_result_s1["std"], marker="x", markersize=4)
plt.errorbar(ct_result_s1["t"], ct_result_s1["avg"], label="Across Continent", yerr=ct_result_s1["std"], marker="x", markersize=4)
plt.errorbar(ac_result_s1["t"], ac_result_s1["avg"], label="Across Atlantic", yerr=ac_result_s1["std"], marker="x", markersize=4)
plt.ylim(bottom=0)
plt.legend()
plt.savefig("TotalTimeS1.png")
plt.show()

# Plot Total Communication for S1
plt.title("Total Communication - Scheme 1")
plt.xlabel("Threshold t")
plt.ylabel("Communication in MB")
plt.grid()
plt.errorbar(dc_result_s1["t"], dc_result_s1["comm"]/1000, label="Same Datacenter", marker="x", markersize=4)
plt.errorbar(ct_result_s1["t"], ct_result_s1["comm"]/1000, label="Across Continent", marker="x", markersize=4)
plt.errorbar(ac_result_s1["t"], ac_result_s1["comm"]/1000, label="Across Atlantic", marker="x", markersize=4)
plt.ylim(bottom=0)
plt.legend()
plt.savefig("TotalCommS1.png")
plt.show()

# Plot Generation Time for S0
plt.title("Share Generation Time - Scheme 1")
plt.xlabel("Threshold t")
plt.ylabel("Time in Milliseconds")
plt.grid()
plt.errorbar(dc_result_s0["t"], dc_result_s0["avg"], label="Same Datacenter", yerr=dc_result_s0["std"], marker="x", markersize=4)
plt.errorbar(ct_result_s0["t"], ct_result_s0["avg"], label="Across Continent", yerr=ct_result_s0["std"], marker="x", markersize=4)
plt.errorbar(ac_result_s0["t"], ac_result_s0["avg"], label="Across Atlantic", yerr=ac_result_s0["std"], marker="x", markersize=4)
plt.ylim(bottom=0)
plt.legend()
plt.savefig("TotalTimeS0.png")
plt.show()

# Plot Total Communication for S0
plt.title("Total Communication - Scheme 1")
plt.xlabel("Threshold t")
plt.ylabel("Communication in MB")
plt.grid()
plt.errorbar(dc_result_s0["t"], dc_result_s0["comm"]/1000, label="Same Datacenter", marker="x", markersize=4)
plt.errorbar(ct_result_s0["t"], ct_result_s0["comm"]/1000, label="Across Continent", marker="x", markersize=4)
plt.errorbar(ac_result_s0["t"], ac_result_s0["comm"]/1000, label="Across Atlantic", marker="x", markersize=4)
plt.ylim(bottom=0)
plt.legend()
plt.savefig("TotalCommS0.png")
plt.show()

