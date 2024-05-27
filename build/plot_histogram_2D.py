import numpy as np
import matplotlib.pyplot as plt

# Read data from the text file
data = np.loadtxt("moduleGridEnergies.txt")

# Extract x, y, and energy values
x = data[:, 1].astype(int)
y = data[:, 2].astype(int)
energy = data[:, 3]

# Determine number of bins in x and y
nbBinsX = len(np.unique(x))
nbBinsY = len(np.unique(y))

# Create a 2D histogram for all modules
hist_data, x_edges, y_edges = np.histogram2d(x, y, bins=[nbBinsX, nbBinsY], weights=energy)

# Plot the histogram
plt.figure(figsize=(8, 6))
plt.imshow(hist_data.T, origin='lower', extent=[x_edges[0], x_edges[-1], y_edges[0], y_edges[-1]], cmap='viridis')
plt.colorbar(label='Energy')
plt.title('Energy Distribution in All Modules')
plt.xlabel('X Grid')
plt.ylabel('Y Grid')
plt.grid(True)
plt.show()

