import matplotlib.pyplot as plt

# Read data from the file
energy_values = []
with open('moduleGridEnergies.txt', 'r') as file:
    for line in file:
        parts = line.split()
        if len(parts) == 4:
            energy = float(parts[3])
            energy_values.append(energy)

# Plot histogram
plt.hist(energy_values, bins=50, edgecolor='black')
plt.xlabel('Energy (units)')
plt.ylabel('Frequency')
plt.title('Histogram of Energy Grid Values')
plt.grid(True)
plt.show()

