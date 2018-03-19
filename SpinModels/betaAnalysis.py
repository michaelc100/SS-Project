#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
import re
import os
from scipy.optimize import curve_fit
from scipy.stats import sem
from commonAnalysis import extractInfo, criticalTemp, logError, func_linear, jackKnife
plt.rc('text', usetex = 'true')
plt.style.use('ggplot')

t, mean, var, sterr, sus, jk, mags = extractInfo(sys.argv[1])

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
premean = mean[0:np.argmax(sus)]

presterr = abs_sterr[0:np.argmax(sus)]
pre_logmean = np.log(premean)
postmean = mean[np.argmax(sus)+1:]
post_logmean = np.log(postmean)
pre_epsilon = epsilon[0:np.argmax(sus)]
log_pre_epsilon = np.log(pre_epsilon)
post_epsilon = epsilon[np.argmax(sus)+1:]
log_post_epsilon = np.log(post_epsilon)

print len(premean), len(presterr)

#only want to fit near critical temperature
stepsBack = 6
stepsForward = 10

fits, covariance = np.polyfit(x = log_pre_epsilon[-stepsBack:], y = pre_logmean[-stepsBack:], deg = 1, cov =  True)
beta = fits[0]
intercept = fits[1]

plt.errorbar(x = log_pre_epsilon, y = pre_logmean, yerr = logError(premean, presterr), fmt = 'bo', markersize = 1)
plt.plot(log_pre_epsilon, beta*log_pre_epsilon + intercept, 'r-', label = r"$\beta = {:.4f} \pm {:.2f}$".format(beta, np.sqrt(np.diag(covariance)[0])))
plt.xlabel(r"$log(\epsilon)$")
plt.ylabel(r"$log( \langle |m| \rangle )$")
plt.legend()
plt.show()
print beta
"""
fits, covariance = np.polyfit(x = log_post_epsilon[:stepsForward], y = post_logmean[:stepsForward], deg = 1, cov =  True)
beta = fits[0]
intercept = fits[1]

plt.errorbar(x = log_post_epsilon, y = post_logmean, yerr = logError(postmean, sterr[np.argmax(sus)+1:]), fmt = 'bo', markersize = 1)
plt.plot(log_post_epsilon, beta*log_post_epsilon + intercept, 'r-', label = r"$Slope = {} \pm {}$".format(beta, np.sqrt(np.diag(covariance)[0])))
plt.xlabel(r"$log(\epsilon)$")
plt.ylabel(r"$log(| \langle m \rangle |)$")
plt.legend()
plt.show()
"""
