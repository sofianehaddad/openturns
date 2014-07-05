#! /usr/bin/env python

from openturns import *
import math as m

TESTPREAMBLE()
RandomGenerator.SetSeed(0)


def buildPoly(p):

    d = p.getDimension()
    center = NumericalPoint(d)
    constant = NumericalPoint(2)
    linear = Matrix(2, d)
    for j in range(d):
        linear[0, j] = p[j]  # z=p1*x1+p2*x2+p3*x3
    constant[1] = 1.0  # sigma
    return LinearNumericalMathFunction(center, constant, linear)


try:

    chainDim = 3
    obsDim = 1
    outputDim = 1

    # observations
    obsSize = 10
    y_obs = NumericalSample(obsSize, obsDim)
    y_obs[0, 0] = -9.50794871493506
    y_obs[1, 0] = -3.83296694500105
    y_obs[2, 0] = -2.44545713047953
    y_obs[3, 0] = 0.0803625289211318
    y_obs[4, 0] = 1.01898069723583
    y_obs[5, 0] = 0.661725805623086
    y_obs[6, 0] = -1.57581204592385
    y_obs[7, 0] = -2.95308465670895
    y_obs[8, 0] = -8.8878164296758
    y_obs[9, 0] = -13.0812290405651
    print 'y_obs=', y_obs

    p = NumericalSample(obsSize, chainDim)
    for i in range(obsSize):
        for j in range(chainDim):
            p[i, j] = (-2 + 5. * i / 9.) ** j
    print 'p=', p

    modelCollection = []
    for i in range(obsSize):
        modelCollection.append(buildPoly(p[i]))
    model = NumericalMathFunction(modelCollection)

    # calibration parameters
    calibrationColl = CalibrationStrategyCollection(chainDim)

    # proposal distribution
    proposalColl = []
    for i in range(chainDim):
        proposalColl.append(Uniform(-1., 1.))

    # prior distribution
    sigma0 = NumericalPoint(chainDim, 10.)  # sigma0= (10.,10.,10.)
    Q0 = CorrelationMatrix(chainDim)  # precision matrix
    Q0_inv = CorrelationMatrix(chainDim)  # variance matrix
    for i in range(chainDim):
        Q0_inv[i, i] = sigma0[i] * sigma0[i]
        Q0[i, i] = 1.0 / Q0_inv[i, i]
    print 'Q0=', Q0

    mu0 = NumericalPoint(chainDim, 0.0)  # mu0 = (0.,0.,0.)
    prior = Normal(mu0, Q0_inv)  # x0 ~ N(mu0, sigma0)
    print 'x~', prior

    # start from te mean x0=(0.,0.,0.)
    print 'x0=', mu0

    # conditional distribution y~N(z, 1.0)
    conditional = Normal()
    print 'y~', conditional

    # create a metropolis-hastings sampler
    sampler = RandomWalkMetropolisHastings(
        prior, conditional, model, y_obs, mu0, proposalColl)
    sampler.setVerbose(True)
    sampler.setThinning(4)
    sampler.setBurnIn(2000)
    sampler.setCalibrationStrategyPerComponent(calibrationColl)

    # get a realization
    realization = sampler.getRealization()
    print 'y1=', realization

    # try to generate a sample
    sampleSize = 1000
    sample = sampler.getSample(sampleSize)

    x_mu = sample.computeMean()
    x_sigma = sample.computeStandardDeviationPerComponent()

    # print acceptance rate
    print 'acceptance rate=', sampler.getAcceptanceRate()

    # compute covariance
    x_cov = sample.computeCovariance()
    P = Matrix(obsSize, chainDim)
    for i in range(obsSize):
        for j in range(chainDim):
            P[i, j] = p[i, j]
    Qn = P.transpose() * P + Q0
    Qn_inv = SquareMatrix(chainDim)
    for j in range(chainDim):
        I_j = NumericalPoint(chainDim)
        I_j[j] = 1.0
        Qn_inv_j = Qn.solveLinearSystem(I_j)
        for i in range(chainDim):
            Qn_inv[i, j] = Qn_inv_j[i]

    sigma_exp = NumericalPoint(chainDim)
    for i in range(chainDim):
        sigma_exp[i] = m.sqrt(Qn_inv[i, i])
    y_vec = NumericalPoint(obsSize)
    for i in range(obsSize):
        y_vec[i] = y_obs[i, 0]

    x_emp = Qn.solveLinearSystem(P.transpose() * y_vec)
    mu_exp = Qn.solveLinearSystem(
        (P.transpose() * P) * x_emp + Matrix(Q0) * mu0)

    print 'sample mean=', x_mu
    print 'expected mean=', mu_exp

    print 'covariance=', x_cov
    print 'expected covariance=', Qn_inv

except:
    import sys
    import traceback
    traceback.print_exc()
