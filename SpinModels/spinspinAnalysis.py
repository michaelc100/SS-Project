#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from commonAnalysis import extractInfo, spinspin, correlation_func_fit, tc_correlation_fit
plt.rc('text', usetex = 'true')
plt.style.use('ggplot')

t, means, mags = extractInfo(sys.argv[1])
ss, errors = spinspin(sys.argv[1], compares = 9) #set compares for no. of spins being correlated

#most of the parameters here should be set according to how the specific data has been generated
print len(ss), len(errors)
print len(ss[0])

sus = np.zeros(len(t))
sus_error = np.zeros(len(t))
for i, mag in enumerate(np.abs(mags)):
	sus[i] = np.var(mag, ddof=1)/t[i]

spins = np.array(ss[10]) #10 index is arbitrary, corresponds to specific temp index being looked at
sterrs = np.array(errors[10])
distance = np.arange(1, len(spins)+1)

#tstar = t.index(2.269)

#popt, pcov = curve_fit(correlation_func_fit, xdata = distance, ydata = spins)
#popt, pcov = curve_fit(tc_correlation_fit, xdata = distance, ydata = spins)
#errors = np.sqrt(np.diag(pcov))
 
#plt.plot(distance, [correlation_func_fit(dist, popt[0], popt[1]) for dist in distance], label = r"$\xi = {}, A = {}$".format(popt[1], popt[0]))
#plt.plot(distance, [tc_correlation_fit(dist, popt[0], popt[1]) for dist in distance], label = r"$\eta = {} \pm {}, \Gamma_{{0}} = {} \pm {}$".format(popt[1], errors[1], popt[0], errors[0]))
plt.errorbar(x = distance, y = spins, yerr = sterrs, fmt = 'bo')
plt.xlabel('Distance from source')
plt.xlim([0, max(distance)+1])
plt.ylabel(r"$\langle s_{i}s_{j} \rangle$")
#plt.legend()
plt.show()

