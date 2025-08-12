
import numpy as np
import matplotlib.pyplot as plt

filename = "output/opt_results.txt"

expected = []
real = []
times = []

correct = 0

with open(filename, 'r') as f:
    for line in f:
        parts = line.strip().split(',')
        if len(parts) != 3:
            continue
        e_val, r_val, t_val = map(float, parts)
        expected.append(e_val)
        real.append(r_val)
        if e_val == r_val:
            correct += 1
        times.append(t_val)

expected = np.array(expected)
real = np.array(real)
times = np.array(times)

# Calculate errors
errors = real - expected
abs_errors = np.abs(errors)
sq_errors = errors**2

print("Analytics Summary:")
print(f"Accuracy: {correct/len(real) * 100}")
print(f"Total Time: {sum(times)}")
print(f"Count: {len(expected)}")
print(f"Mean Error: {np.mean(errors):.5f}")
print(f"Mean Absolute Error (MAE): {np.mean(abs_errors):.5f}")
print(f"Mean Squared Error (MSE): {np.mean(sq_errors):.5f}")
print(f"Root Mean Squared Error (RMSE): {np.sqrt(np.mean(sq_errors)):.5f}")
print(f"Average time: {np.mean(times):.5f} units")
print(f"Time standard deviation: {np.std(times):.5f} units")

# Optional plots
plt.figure(figsize=(10, 4))

plt.subplot(1, 2, 1)
plt.plot(expected, label='Expected', marker='o')
plt.plot(real, label='Real', marker='x')
plt.legend()
plt.title("Expected vs Real Values")

plt.subplot(1, 2, 2)
plt.hist(errors, bins=30, alpha=0.7, color='orange')
plt.title("Error Distribution")

plt.tight_layout()
plt.show()
