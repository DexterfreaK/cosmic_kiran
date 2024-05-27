import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def read_energy_data(file_path):
    data = []
    with open(file_path, 'r') as file:
        for line in file:
            module, gridX, gridY, energy = map(float, line.split())
            data.append([module, gridX, gridY, energy])
    return np.array(data)

def plot_energy_3d(data, module_number):
    module_data = data[data[:, 0] == module_number]
    if module_data.size == 0:
        print(f"No data found for module number {module_number}")
        return
    
    gridX = module_data[:, 1].astype(int)
    gridY = module_data[:, 2].astype(int)
    energy = module_data[:, 3]
    
    grid_size_x = max(gridX) + 1
    grid_size_y = max(gridY) + 1
    energy_grid = np.zeros((grid_size_x, grid_size_y))
    
    for x, y, e in zip(gridX, gridY, energy):
        energy_grid[x, y] = e

    X, Y = np.meshgrid(np.arange(grid_size_x), np.arange(grid_size_y))
    Z = energy_grid[X, Y]

    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(X, Y, Z, cmap='viridis')
    fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5, label='Energy (MeV)')
    ax.set_xlabel('Grid X')
    ax.set_ylabel('Grid Y')
    ax.set_zlabel('Energy (MeV)')
    ax.set_title(f'3D Energy Distribution for Module {module_number}')
    plt.show()

def main():
    file_path = 'moduleGridEnergies.txt'
    data = read_energy_data(file_path)
    
    module_number = 3  # Change this to plot different modules if needed
    plot_energy_3d(data, module_number)

if __name__ == "__main__":
    main()

