#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
import re
import os
from scipy.optimize import curve_fit
from scipy.stats import sem
import commonAnalysis as ca
"""
Script to retrieve info about mc simulation of random graphs from graph evolution
Then calculates relevant info and produces various plots given these .txt files
"""

def criticalTemp(filePath):
	t, m, v, st, su, jk, ma, dv = ca.extractInfo(filePath)		
	mags = np.array(ma)
	abs_mags = np.abs(mags)
	abs_sterr = sem(abs_mags, axis = 1)
	sus = np.zeros(len(t))
	sus_error = np.zeros(len(t))
	for i, mag in enumerate(abs_mags):
		sus[i] = np.var(mag, ddof=1)/t[i]
		sus_error[i] = ca.jackKnife(mag, np.var)

	return t[np.argmax(sus)]

#needed for fitting curve
def func_linear(x, m, c):
	return c + m*x

def beta(filePath, stepsBack = 8):
	t, m, v, st, su, jk, ma, dv = extractInfo(filePath)
	tc = criticalTemp(filePath)
	mags = np.array(ma)
	abs_mags = np.abs(mags)
	abs_sterr = sem(abs_mags, axis = 1)
	mean = np.mean(abs_mags, axis = 1)
	tc_index = t.index(tc)
	epsilon = np.absolute(1-tc/np.array(t))

	premean = mean[0:tc_index]
	presterr = abs_sterr[0:tc_index]
	pre_logmean = np.log(premean)
	postmean = mean[tc_index+1:]
	post_logmean = np.log(postmean)
	pre_epsilon = epsilon[0:tc_index]
	log_pre_epsilon = np.log(pre_epsilon)
	post_epsilon = epsilon[tc_index+1:]
	log_post_epsilon = np.log(post_epsilon)
	fits, covariance = np.polyfit(x = log_pre_epsilon[-stepsBack:], y = pre_logmean[-stepsBack:], deg = 1, cov =  True)
	beta = fits[0]
	intercept = fits[1]
	return [beta, np.sqrt(np.diag(covariance))[0]]
		
def gamma(filePath, stepsForward = 12):
	t, m, v, st, su, jk, ma, dv = extractInfo(filePath)
	tc = criticalTemp(filePath)
	mags = np.array(ma)
	abs_mags = np.abs(mags)
	abs_sterr = sem(abs_mags, axis = 1)
	mean = np.mean(abs_mags, axis = 1)
	tc_index = t.index(tc)
	epsilon = np.absolute(1-tc/np.array(t))
	sus = np.zeros(len(t))
	sus_error = np.zeros(len(t))
	for i, mag in enumerate(np.abs(mags)):
		sus[i] = np.var(mag, ddof=1)/t[i]
		sus_error[i] = ca.jackKnife(mag, np.var)
	postsus = sus[tc_index+1:]
	post_logsus = np.log(postsus)
	post_epsilon = epsilon[tc_index+1:]
	log_post_epsilon = np.log(post_epsilon)
	fits, covariance = np.polyfit(x = log_post_epsilon[:stepsForward], y = post_logsus[:stepsForward], deg = 1, cov =  True)
	gamma = -fits[0]
	intercept = fits[1]
	return [gamma, np.sqrt(np.diag(covariance))[0]]

variance = np.zeros(10)
temps = np.zeros(10)
betas = np.zeros(10)
beta_errors = np.zeros(10)
gammas = np.zeros(10)
gamma_errors = np.zeros(10)

sb = 6

for i, file in enumerate(os.listdir(os.getcwd()+"/cebatch")):
	if i > 0: 
		sb = 10
	if file.endswith(".txt"):
		t, m, v, st, su, jk, ma, dv = ca.extractInfo(os.getcwd()+"/cebatch/"+file)
		variance[i] = dv
		temps[i] = criticalTemp(os.getcwd()+"/cebatch/"+file)
		betaInfo = beta(os.getcwd()+"/cebatch/"+file, sb)
		betas[i] = betaInfo[0]
		beta_errors[i] = betaInfo[1]
		gammaInfo = gamma(os.getcwd()+"/cebatch/"+file)
		gammas[i] = gammaInfo[0]
		gamma_errors = gammaInfo[1]

plt.style.use('ggplot')
plt.rc('text', usetex = 'true')

plt.errorbar(variance, temps, yerr = [0.1]*len(temps), fmt = 'bo')
plt.xlabel('Vertex Degree Variance')
plt.ylim([2.0,4.0])
plt.xlim([-0.1, 4.0])
plt.ylabel('Critical Temperature')
plt.show() 

plt.errorbar(variance, betas, yerr = beta_errors, fmt = 'bo')
plt.xlabel('Vertex Degree Variance')
plt.xlim([-0.1, 4.0])
plt.ylabel(r"$\beta$")
plt.show()

plt.errorbar(variance, gammas, yerr = gamma_errors, fmt = 'bo')
plt.xlabel('Vertex Degree Variance')
plt.xlim([-0.1, 4.0])
plt.ylabel(r"$\gamma$")
plt.show()




