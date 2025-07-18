import matplotlib.pyplot as plt
import pandas as pd

# Load data
df = pd.read_csv('DHT22_data.csv', names=["Timestamp", "Temperature", "Humidity"])

# Convert Timestamp to datetime
df["Timestamp"] = pd.to_datetime(df["Timestamp"], format='%Y%m%d%H%M%S')

# Plot
plt.figure(figsize=(10,5))
plt.plot(df["Timestamp"], df["Temperature"], label="Temperature (°C)")
plt.plot(df["Timestamp"], df["Humidity"], label="Humidity (%)")
plt.xlabel("Time")
plt.ylabel("Values")
plt.title("DHT22 Temperature and Humidity Over Time")
plt.legend()
plt.grid(True)
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()
