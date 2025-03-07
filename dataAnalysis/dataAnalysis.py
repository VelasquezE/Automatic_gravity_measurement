import numpy as np
import uncertainties as u 
from uncertainties import unumpy

ROPE_LENGHT = u.ufloat(0.265, 0.0005) # [m]
ACCEPTED_GRAVITY = 9.774 # [m/s^2]

period_data = np.genfromtxt("dataArduino.txt")

# Remove outliers
first_quartile = np.percentile(period_data, 25)  
third_quartile = np.percentile(period_data, 75)  
interquartile_range = third_quartile - first_quartile  
lower_limit = first_quartile - 1.5 * interquartile_range  
upper_limit = third_quartile + 1.5 * interquartile_range
period_data_filtered = period_data[(period_data >= lower_limit) & (period_data <= upper_limit)]

period_mean = np.mean(period_data_filtered)
period_uncertainty = np.std(period_data_filtered, ddof=1) 

period_with_uncertainty = u.ufloat(period_mean, period_uncertainty)

def calculate_gravity(period):
    gravity = (4 * np.pi * np.pi * ROPE_LENGHT) / (period * period)
    return gravity

gravity = calculate_gravity(period_mean)

error = (np.absolute(gravity.nominal_value - ACCEPTED_GRAVITY) / ACCEPTED_GRAVITY) * 100

# Mostrar resultados
print(f"Gravedad medida: ({gravity}) m/s^2 \n")
print(f"{error:.2f}% de error respecto al valor aceptado de 9.774 m/s^2")



