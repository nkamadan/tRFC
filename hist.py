import matplotlib.pyplot as plt

# Read the file and split it into two lists: times and latencies
times = []
latencies = []

with open('histogram.txt', 'r') as f:
    for line in f:
        time, latency = line.split()
        times.append(int(time))
        latencies.append(int(latency))

# Plotting the data
plt.plot(times, latencies, '-o', markersize=3)
plt.xlabel('Time')
plt.ylabel('Latency (clock cycles)')
plt.title('Latency vs Time')
plt.grid(True)
plt.tight_layout()

# Display the plot
plt.show()
