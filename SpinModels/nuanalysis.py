#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d
from scipy.optimize import curve_fit
from scipy.stats import sem
from commonAnalysis import extractInfo, criticalTemp, spinspin, correlation_func_fit, tc_correlation_fit, logError, func_linear
plt.rc('text', usetex = 'true')
plt.style.use('ggplot')

def xi(spins, distance): #perform curve_fit at a given temp to get correlation length
	popt, pcov = curve_fit(correlation_func_fit, xdata = distance, ydata = spins, bounds = (0,1), method = 'trf')
	errors = np.sqrt(np.diag(pcov))
	return [popt[1], errors[1]]

t, means, var, sterr, sus, jk, mags = extractInfo(sys.argv[1])

compares_rand = 9
compares_square = 20
ss, errors = spinspin(sys.argv[1], compares = compares_rand)

abs_mags = np.abs(mags)
mean = np.mean(abs_mags, axis = 1)
abs_sterr = sem(abs_mags, axis = 1)
sus = np.zeros(len(t))
sus_error = np.zeros(len(t))
for i, mag in enumerate(np.abs(mags)):
	sus[i] = np.var(mag, ddof=1)/t[i]

tc = t[np.argmax(sus)]
print t
ss = np.array(ss)

print ss
spins = np.array(ss[np.argmax(sus)])
sterrs = np.array(errors[np.argmax(sus)])
distance = np.arange(1, len(spins)+1)

#plot <sisj> for a fixed t
plt.errorbar(y = spins, x = distance, yerr = sterrs, fmt = 'ro')
plt.xlim([0, max(distance)+1])
plt.xlabel(r"$d(i, j)$")
plt.ylabel(r"$\langle s_i s_j  \rangle$")
plt.show()
#now want to make 3d plot of t, <sisj> and d
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i, temp in enumerate(t):
	ax.plot3D(distance, temp*np.ones(len(distance)), zs = ss[i])
	for j, d in enumerate(distance):
		ax.plot3D([d, d], [temp, temp], [ss[i][j]+errors[i][j], ss[i][j]-errors[i][j]], marker="_")

#ax.legend()
ax.set_zlabel(r"$\langle s_i s_j \rangle$")
plt.xlabel(r"$d(i, j)$")
#plt.zlabel(r"$\langle s_i s_j \rangle$")
plt.ylabel(r"$T$")
plt.show()

popt_tc, pcov_tc = curve_fit(correlation_func_fit, xdata = distance, ydata = ss[-4], bounds = (0,1), method = 'trf')
plt.errorbar(distance, ss[-4], yerr = errors[-4], fmt = 'bo')
plt.plot(distance, [correlation_func_fit(d, popt_tc[0], popt_tc[1], popt_tc[2]) for d in distance], 'r-', label = r"$\xi = {:.4f} \pm {:.2f}, d = {:.4f} \pm {:.2f}, T = {}$".format(popt_tc[1], np.sqrt(np.diag(pcov_tc))[1], popt_tc[2], np.sqrt(np.diag(pcov_tc))[2], t[-4]))
plt.xlabel(r"$d(i, j)$")
plt.ylabel(r"$\langle s_i s_j  \rangle$")
plt.xlim([0,10])
plt.legend()
plt.show()

xivals = np.array([xi(s, np.arange(1, compares_rand+1))[0] for s in ss])
xierrors = np.array([xi(s, np.arange(1, compares_rand+1))[1] for s in ss])

plt.errorbar(x = t[np.argmax(sus)+1:], y = xivals[np.argmax(sus)+1:], yerr = xierrors[np.argmax(sus)+1:], fmt = 'bo')
plt.ylabel(r"$\xi$")
plt.xlabel(r"$T$")
plt.show()

epsilon = np.absolute(1-tc/np.array(t))
logxi = np.log(xivals[np.argmax(sus)+1:])
logeps = np.log(epsilon[np.argmax(sus)+1:])
#popt_log, pcov_log = curve_fit(func_linear, logeps, logxi)

plt.errorbar(logeps, logxi, yerr = logError(xivals, xierrors)[np.argmax(sus)+1:], fmt = 'bo', label = r"$\nu = {:.4f} \pm {:.2f}$".format(-popt_log[0], np.sqrt(np.diag(pcov_log))[0]))
#plt.plot(logeps, popt_log[1]+popt_log[0]*logeps, 'r-')
plt.xlabel(r"$log(\epsilon)$")
plt.ylabel(r"$log(\xi)$")
plt.legend()
plt.show()


