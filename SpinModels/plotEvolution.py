#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
import re
import os
from scipy.optimize import curve_fit
from scipy.stats import sem
from matplotlib import rc
import commonAnalysis as ca
plt.rc('text', usetex=True)
plt.style.use('ggplot')
"""
Script to retrieve info about mc simulation of random graphs from graph evolution
Then calculates relevant info and produces various plots given these .txt files
"""
t1, m1, mags1 = ca.extractInfo(sys.argv[1])

abs_mags1 = np.abs(mags1)
abs_sterr = sem(abs_mags1, axis = 1)
sus = np.zeros(len(t1))
sus_error = np.zeros(len(t1))
for i, mag in enumerate(abs_mags1):
	sus[i] = np.var(mag, ddof=1)
	sus_error[i] = ca.jackKnife(mag, np.var)

plt.errorbar(t1, sus, yerr = sus_error, fmt = 'bo', markersize = '4')
plt.plot(t1[np.argmax(sus)], max(sus), 'r^', label = r"$T_C = {} \pm {}$".format(t1[np.argmax(sus)], t1[1]-t1[0]))
plt.xlabel(r"$T$")
plt.ylabel(r"$\chi$")
plt.legend()
plt.show()

plt.errorbar(t1, np.mean(abs_mags1, axis = 1), yerr = abs_sterr, fmt = 'ro', markersize = 1, label = '4')
plt.xlabel(r'$T$')
plt.ylabel(r"$\|m(T)\|$")
#plt.title(r"Phase change of random graph with connectivity 4, $\|V\|=500$")
plt.show()
	
bcerror = [ca.jackKnife(m, ca.binderCumulant) for m in mags1]

plt.errorbar(t1, [ca.binderCumulant(m) for m in mags1] , yerr = bcerror, fmt = 'bo', markersize = 3)
plt.xlabel('Temperature')
plt.ylabel('Binder Cumulant')
plt.plot(t1, (2/3.0)*(np.ones(len(t1))), 'r-', label = r"$\frac{2}{3}$")
#plt.legend()
plt.show()

