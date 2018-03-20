#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
import re
import os
from scipy.stats import sem
from scipy.optimize import curve_fit
from commonAnalysis import extractInfo, logError, func_linear, jackKnife

plt.rc('text', usetex = 'true')
plt.style.use('ggplot')

t, mean, mags = extractInfo(sys.argv[1])

abs_mags = np.abs(mags)
mean = np.mean(abs_mags, axis = 1)
abs_sterr = sem(abs_mags, axis = 1)
sus = np.zeros(len(t))
sus_error = np.zeros(len(t))
for i, mag in enumerate(np.abs(mags)):
	sus[i] = np.var(mag, ddof=1)/t[i]
	sus_error[i] = jackKnife(mag, np.var)
tc = t[np.argmax(sus)]

epsilon = np.absolute(1-tc/np.array(t))
presus = sus[0:np.argmax(sus)]
pre_logsus = np.log(presus)
postsus = sus[np.argmax(sus)+1:]
post_logsus = np.log(postsus)
pre_epsilon = epsilon[0:np.argmax(sus)]
log_pre_epsilon = np.log(pre_epsilon)
post_epsilon = epsilon[np.argmax(sus)+1:]
log_post_epsilon = np.log(post_epsilon)

stepsBack = 6
stepsForward = 11

plt.errorbar(x = t, y = sus, yerr = sus_error, fmt = 'bo', markersize = 3)
plt.xlabel(r"$T$")
plt.ylabel(r"$\chi$")
plt.show()

#fits, covariance = np.polyfit(x = log_pre_epsilon[-stepsBack:], y = pre_logsus[-stepsBack:], deg = 1, cov =  True)
fits, covariance = np.polyfit(x = log_post_epsilon[:stepsForward], y = post_logsus[:stepsForward], deg = 1, cov =  True)
gamma = fits[0]
intercept = fits[1]

#plt.errorbar(x = log_pre_epsilon, y = pre_logsus, fmt = 'bo', markersize = 3)
plt.errorbar(x = log_post_epsilon, y = post_logsus, fmt = 'bo', markersize = 3)
#plt.plot(log_pre_epsilon, gamma*log_pre_epsilon + intercept, 'r-', label = r"$Slope = {:.4f} \pm {:.2f}$".format(gamma, np.sqrt(np.diag(covariance)[0])))
plt.plot(log_post_epsilon, gamma*log_post_epsilon + intercept, 'r-', label = r"$Slope = {:.4f} \pm {:.2f}$".format(gamma, np.sqrt(np.diag(covariance)[0])))
plt.xlabel(r"$log(\epsilon)$")
plt.ylabel(r"$log(\chi)$")
plt.legend()
plt.show()

