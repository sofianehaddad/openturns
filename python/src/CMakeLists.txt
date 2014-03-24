#                                               -*- cmake -*-

add_definitions ( ${OPENTURNS_SWIG_DEFINITIONS} )

# generate SWIG runtime header
set ( SWIG_PYTHON_OPTIONS "-python" )
add_custom_command ( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/swig_runtime.hxx
                     COMMAND ${SWIG_EXECUTABLE}
                     ARGS ${SWIG_PYTHON_OPTIONS} ${CMAKE_SWIG_FLAGS} ${OPENTURNS_SWIG_DEFINITIONS} -external-runtime ${CMAKE_CURRENT_BINARY_DIR}/swig_runtime.hxx
                     COMMENT "Swig header" )
add_custom_target ( generate_swig_runtime DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/swig_runtime.hxx )
include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

include_directories ( ${INTERNAL_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

set ( OPENTURNS_PYTHON_MODULES )
macro ( ot_add_python_module MODULENAME SOURCEFILE )
  set_source_files_properties ( ${SOURCEFILE} PROPERTIES CPLUSPLUS ON )
  set_source_files_properties ( ${SOURCEFILE} PROPERTIES SWIG_MODULE_NAME ${MODULENAME} )
  ot_install_swig_file ( ${SOURCEFILE} )
  if ( ${ARGC} GREATER 2 )
    set ( SWIG_MODULE_${MODULENAME}_EXTRA_DEPS ${ARGN} )
  endif ()
    
  set ( swig_other_sources )
  foreach ( it ${ARGN} )
    if ( ${it} MATCHES ".*\\.i$" )
      ot_install_swig_file ( ${it} )
    else ()
      set ( swig_other_sources ${swig_other_sources} "${it}" )
    endif ()
  endforeach ()

  # workaround for a bug in UseSWIG script not supporting multiple modules and parallel compilation
  # http://www.cmake.org/pipermail/cmake/2011-January/042345.html
  if ( CMAKE_VERSION VERSION_LESS 2.8.5 )
    set( swig_extra_generated_files )
  endif ()
  
  swig_add_module ( ${MODULENAME} python ${SOURCEFILE} ${swig_other_sources} )

  add_dependencies ( ${SWIG_MODULE_${MODULENAME}_REAL_NAME} generate_swig_runtime )
  swig_link_libraries ( ${MODULENAME} OT )
  
  if ( LINK_PYTHON_LIBRARY )
    swig_link_libraries ( ${MODULENAME} ${PYTHON_LIBRARIES} )
  endif ()
  
  set_target_properties ( ${SWIG_MODULE_${MODULENAME}_REAL_NAME} PROPERTIES NO_SONAME ON )
  
  if ( MINGW )
    set_target_properties ( ${SWIG_MODULE_${MODULENAME}_REAL_NAME} PROPERTIES
                                                                   LINK_FLAGS "-shared ${CMAKE_EXE_LINKER_FLAGS}" )
  endif ()
  list ( APPEND OPENTURNS_PYTHON_MODULES ${MODULENAME} )
endmacro ( ot_add_python_module )

install ( FILES PythonNumericalMathEvaluationImplementation.hxx
                PythonRandomVectorImplementation.hxx
                PythonDistributionImplementation.hxx
                PythonWrappingFunctions.hxx
          DESTINATION ${OPENTURNS_INCLUDE_PATH}
        )

if ( LIBXML2_FOUND )
  list ( APPEND SWIG_MODULE_common_EXTRA_FLAGS -DHAVE_LIBXML2=1 )
endif ()
if ( MUPARSER_FOUND )
  list ( APPEND SWIG_MODULE_func_EXTRA_FLAGS -DOPENTURNS_HAVE_MUPARSER=1 )
endif ()
ot_add_python_module( common common_module.i
                      OTtypes.i
                      OTexceptions.i
                      Memory.i
                      Object.i
                      PersistentObject.i
                      Study.i
                      StorageManager.i
                      XMLStorageManager.i
                      Log.i
                      MPI.i
                      TTY.i
                      Path.i
                      Catalog.i
                      ResourceMap.i
                      ComparisonOperatorImplementation.i
                      ComparisonOperator.i
                      Equal.i
                      Greater.i
                      GreaterOrEqual.i
                      Less.i
                      LessOrEqual.i
                      Cache.i
                      PlatformInfo.i
                      SharedPointer.i
                      Pointer.i
                      InterfaceObject.i
                      TypedInterfaceObject.i
                      TypedCollectionInterfaceObject.i
                      BaseCommonTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                     
ot_add_python_module( wrapper wrapper_module.i
                      Collection.i
                      WrapperData.i
                      WrapperFile.i
                      BaseWrapperTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( typ typ_module.i
                      PersistentCollection.i
                      DescriptionImplementation.i
                      Description.i
                      NumericalPoint.i
                      NumericalPointWithDescription.i
                      Indices.i
                      MatrixImplementation.i
                      Matrix.i
                      SquareMatrix.i
                      SymmetricMatrix.i
                      TensorImplementation.i
                      Tensor.i
                      SymmetricTensor.i
                      BaseTypTemplateDefs.i
                      ComplexMatrixImplementation.i
                      ComplexMatrix.i
                      HermitianMatrix.i
                      SquareComplexMatrix.i
                      TriangularMatrix.i
                      TriangularComplexMatrix.i
                      CovarianceMatrix.i
                      CorrelationMatrix.i
                      IdentityMatrix.i
                      NumericalSample.i
                      HistoryStrategyImplementation.i
                      HistoryStrategy.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( statistics statistics_module.i
                      Compact.i
                      Full.i
                      Last.i
                      Null.i
                      ConfidenceInterval.i
                      LinearModel.i
                      LinearModelFactory.i
                      TestResult.i
                      ProcessSample.i
                      RandomGenerator.i
                      RandomGeneratorState.i
                      CorrelationAnalysis.i
                      SensitivityAnalysis.i
                      LowDiscrepancySequenceImplementation.i
                      LowDiscrepancySequence.i
                      SobolSequence.i
                      FaureSequence.i
                      HaltonSequence.i
                      HaselgroveSequence.i
                      ReverseHaltonSequence.i
                      Field.i
                      FieldImplementation.i
                      TimeSeries.i
                      BaseStatisticsTemplateDefs.i
                      SecondOrderModelImplementation.i
                      SecondOrderModel.i
                      ExponentialCauchy.i
                      FilteringWindowsImplementation.i
                      FilteringWindows.i
                      Hamming.i
                      Hanning.i
                      SpectralModelImplementation.i
                      SpectralModel.i
                      CovarianceModelImplementation.i
                      CovarianceModel.i
                      StationaryCovarianceModel.i
                      SpectralModelFactoryImplementation.i
                      SpectralModelFactory.i
                      UserDefinedSpectralModel.i
                      WelchFactory.i
                      ExponentialModel.i
                      ExponentiallyDampedCosineModel.i
                      CauchyModel.i
                      CovarianceModelFactoryImplementation.i
                      CovarianceModelFactory.i
                      UserDefinedCovarianceModel.i
                      UserDefinedStationaryCovarianceModel.i
                      StationaryCovarianceModelFactory.i
                      NonStationaryCovarianceModelFactory.i
                      SquaredExponential.i
                      GeneralizedExponential.i
                      AbsoluteExponential.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( geom geom_module.i
                      DomainImplementation.i
                      Mesh.i
                      Domain.i
                      Interval.i
                      MeshFactoryImplementation.i
                      MeshFactory.i
		      RegularGrid.i
                      BaseGeomTemplateDefs.i
                     )
                     
ot_add_python_module( graph graph_module.i
                      DrawableImplementation.i
                      Drawable.i
                      GraphImplementation.i
                      Graph.i
                      BarPlot.i
                      Cloud.i
                      Contour.i
                      Curve.i
                      Pairs.i
                      Pie.i
                      Polygon.i
                      Staircase.i
                      BaseGraphTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( func func_module.i
                      SpecFunc.i
                      NumericalMathEvaluationImplementation.i
                      NumericalMathGradientImplementation.i
                      NumericalMathHessianImplementation.i
                      NoNumericalMathEvaluationImplementation.i
                      NoNumericalMathGradientImplementation.i
                      NoNumericalMathHessianImplementation.i
                      LinearNumericalMathEvaluationImplementation.i
                      LinearNumericalMathGradientImplementation.i
                      QuadraticNumericalMathEvaluationImplementation.i
                      ConstantNumericalMathGradientImplementation.i
                      ConstantNumericalMathHessianImplementation.i
                      ComputedNumericalMathEvaluationImplementation.i
                      ComputedNumericalMathGradientImplementation.i
                      ComputedNumericalMathHessianImplementation.i
                      ComposedNumericalMathEvaluationImplementation.i
                      ComposedNumericalMathGradientImplementation.i
                      ComposedNumericalMathHessianImplementation.i
                      DualLinearCombinationEvaluationImplementation.i
                      DualLinearCombinationGradientImplementation.i
                      DualLinearCombinationHessianImplementation.i
                      LinearCombinationEvaluationImplementation.i
                      LinearCombinationGradientImplementation.i
                      LinearCombinationHessianImplementation.i
                      ProductNumericalMathEvaluationImplementation.i
                      ProductNumericalMathGradientImplementation.i
                      ProductNumericalMathHessianImplementation.i
                      NumericalMathFunctionImplementation.i
                      ComposedNumericalMathFunction.i
                      ProductNumericalMathFunction.i
                      NumericalMathFunction.i
                      BaseFuncCollection.i
                      DynamicalFunctionImplementation.i
                      DynamicalFunction.i
                      SpatialFunction.i
                      TemporalFunction.i
                      LinearNumericalMathFunction.i
                      QuadraticNumericalMathFunction.i
                      UniVariatePolynomialImplementation.i
                      UniVariatePolynomial.i
                      AggregatedNumericalMathEvaluationImplementation.i
                      AnalyticalNumericalMathEvaluationImplementation.i
                      AnalyticalNumericalMathGradientImplementation.i
                      AnalyticalNumericalMathHessianImplementation.i
                      DatabaseNumericalMathEvaluationImplementation.i
                      IndicatorNumericalMathEvaluationImplementation.i
                      PiecewiseLinearEvaluationImplementation.i
                      PiecewiseHermiteEvaluationImplementation.i
                      ProductPolynomialEvaluationImplementation.i
                      RestrictedEvaluationImplementation.i
                      RestrictedGradientImplementation.i
                      RestrictedHessianImplementation.i
                      Basis.i
                      BasisSequenceImplementation.i
                      BasisSequence.i
                      BasisSequenceFactoryImplementation.i
                      BasisSequenceFactory.i
                      LAR.i
                      BaseFuncTemplateDefs.i 
                      BoxCoxEvaluationImplementation.i
                      BoxCoxTransform.i
                      InverseBoxCoxEvaluationImplementation.i
                      PythonNumericalMathEvaluationImplementation.cxx
                      InverseBoxCoxTransform.i
                      TrendEvaluationImplementation.i
                      TrendTransform.i
                      InverseTrendEvaluationImplementation.i
                      InverseTrendTransform.i
                      ExpertMixture.i
                      BasisFactory.i
                      ConstantBasisFactory.i
                      LinearBasisFactory.i
                      QuadraticBasisFactory.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( diff diff_module.i
                      FiniteDifferenceStepImplementation.i
                      FiniteDifferenceStep.i
                      ConstantStep.i
                      BlendedStep.i
                      FiniteDifferenceGradient.i
                      FiniteDifferenceHessian.i
                      CenteredFiniteDifferenceGradient.i
                      CenteredFiniteDifferenceHessian.i
                      NonCenteredFiniteDifferenceGradient.i
                      BaseDiffTemplateDefs.i
                      PythonWrappingFunctions.hxx
                     )

ot_add_python_module( optim optim_module.i
                      BoundConstrainedAlgorithmImplementationResult.i
                      BoundConstrainedAlgorithmImplementation.i
                      BoundConstrainedAlgorithm.i
                      NearestPointAlgorithmImplementationResult.i
                      NearestPointAlgorithmImplementation.i
                      NearestPointAlgorithm.i
                      NearestPointCheckerResult.i
                      NearestPointChecker.i
                      AbdoRackwitzSpecificParameters.i
                      AbdoRackwitz.i
                      CobylaSpecificParameters.i
                      Cobyla.i
                      TNCSpecificParameters.i
                      TNC.i
                      SQPSpecificParameters.i
                      SQP.i
                      BaseOptimTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( solver solver_module.i
                      SolverImplementation.i
                      Solver.i
                      Bisection.i
                      Secant.i
                      Brent.i
                      BaseSolverTemplateDefs.i
                      ODESolverImplementation.i
                      ODESolver.i
                      RungeKutta.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( algo algo_module.i
                      ApproximationAlgorithmImplementation.i
                      ApproximationAlgorithmImplementationFactory.i
                      ApproximationAlgorithm.i
                      ClassifierImplementation.i
                      Classifier.i
                      FittingAlgorithmImplementation.i
                      FittingAlgorithm.i
                      CorrectedLeaveOneOut.i
                      KFold.i
                      LeastSquaresMetaModelSelection.i
                      LeastSquaresMetaModelSelectionFactory.i
                      PenalizedLeastSquaresAlgorithm.i
                      PenalizedLeastSquaresAlgorithmFactory.i
                      PythonNumericalMathEvaluationImplementation.cxx
                      BaseAlgoTemplateDefs.i
                      FFTImplementation.i
                      FFT.i
                      KissFFT.i
                      IntegrationAlgorithmImplementation.i
                      IntegrationAlgorithm.i
                      GaussKronrodRule.i
                      GaussKronrod.i
                      PythonWrappingFunctions.hxx
                    )
                     
ot_add_python_module( experiment experiment_module.i
                      ExperimentImplementation.i
                      StratifiedExperiment.i
                      Experiment.i
                      Axial.i
                      Composite.i
                      Factorial.i
                      Box.i
                      CombinatorialGeneratorImplementation.i
                      CombinatorialGenerator.i
                      Combinations.i
                      KPermutations.i
                      Tuples.i
                      BaseExperimentTemplateDefs.i
                    )

ot_add_python_module( base base_module.i
                      BaseTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( model_copula model_copula_module.i
                      DistributionImplementation.i
                      DistributionImplementationFactory.i
                      Distribution.i
                      DistributionFactory.i
                      ContinuousDistribution.i
                      EllipticalDistribution.i
                      DiscreteDistribution.i
                      CopulaImplementation.i
                      Copula.i
                      ArchimedeanCopula.i
                      SklarCopula.i
                      IndependentCopula.i
                      MinCopula.i
                      NormalCopula.i
                      NormalCopulaFactory.i
                      FarlieGumbelMorgensternCopula.i
                      FarlieGumbelMorgensternCopulaFactory.i
                      AliMikhailHaqCopula.i
                      AliMikhailHaqCopulaFactory.i
                      FrankCopula.i
                      FrankCopulaFactory.i
                      ClaytonCopula.i
                      ClaytonCopulaFactory.i
                      GumbelCopula.i
                      GumbelCopulaFactory.i
                      ComposedDistribution.i
                      ComposedCopula.i
                      CompositeDistribution.i
                      ConditionalDistribution.i
                      PosteriorDistribution.i
                      PythonDistributionImplementation.cxx
                      UncertaintyModelCopulaCollection.i
                      UncertaintyModelCopulaTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( randomvector randomvector_module.i
                      ProcessImplementation.i
                      Process.i
                      RandomVectorImplementation.i
                      ConstantRandomVector.i
                      UsualRandomVector.i
                      UncertaintyRandomVectorTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( model_process model_process_module.i
                      UncertaintyProcessTemplateDefs.i
                      WhiteNoise.i
                      RandomWalk.i
                      ARMACoefficients.i
                      ARMAState.i
                      ARMA.i
                      SpectralNormalProcess.i
                      TemporalNormalProcess.i
                      CompositeProcess.i
                      FunctionalBasisProcess.i
                      ARMAFactoryImplementation.i
                      ARMAFactory.i
                      WhittleFactory.i
                      WhittleFactoryState.i
                      PythonWrappingFunctions.hxx
                      ARMALikelihoodFactory.i    
                    )

ot_add_python_module( dist_bundle1 dist_bundle1_module.i
                      DistFunc.i
                      Arcsine.i
                      ArcsineFactory.i
                      Bernoulli.i
                      BernoulliFactory.i
                      Beta.i
                      BetaFactory.i
                      Binomial.i
                      BinomialFactory.i
                      Burr.i
                      BurrFactory.i
                      ChiSquare.i
                      ChiSquareFactory.i
                      Chi.i
                      ChiFactory.i
                      Dirac.i
                      DiracFactory.i
                      Dirichlet.i
                      DirichletFactory.i
                      Epanechnikov.i
                      Exponential.i
                      ExponentialFactory.i
                      FisherSnedecor.i
                      FisherSnedecorFactory.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( dist_bundle2 dist_bundle2_module.i                 
                      Gamma.i
                      GammaFactory.i
                      GeneralizedPareto.i
                      GeneralizedParetoFactory.i
                      Geometric.i
                      GeometricFactory.i
                      Gumbel.i
                      GumbelFactory.i
                      HistogramPair.i
                      Histogram.i
                      HistogramFactory.i
                      InverseNormal.i
                      InverseNormalFactory.i
                      KPermutationsDistribution.i
                      KernelMixture.i
                      KernelSmoothing.i
                      Laplace.i
                      LaplaceFactory.i
                      Logistic.i
                      LogisticFactory.i
                      LogNormal.i
                      LogNormalFactory.i
                      LogUniform.i
                      LogUniformFactory.i
                      MeixnerDistribution.i
                      MeixnerDistributionFactory.i
                      Mixture.i
                      Multinomial.i
                      MultinomialFactory.i
                      PythonWrappingFunctions.hxx
                    )
                    
ot_add_python_module( dist_bundle3 dist_bundle3_module.i
                      NegativeBinomial.i
                      NegativeBinomialFactory.i
                      NonCentralChiSquare.i
                      NonCentralStudent.i
                      Normal.i
                      NormalFactory.i
                      Poisson.i
                      PoissonFactory.i
                      RandomMixture.i
                      Rayleigh.i
                      RayleighFactory.i
                      Rice.i
                      RiceFactory.i
                      Skellam.i
                      SkellamFactory.i
                      Student.i
                      StudentFactory.i
                      Trapezoidal.i
                      TrapezoidalFactory.i
                      Triangular.i
                      TriangularFactory.i
                      TruncatedDistribution.i
                      TruncatedNormal.i
                      TruncatedNormalFactory.i
                      Uniform.i
                      UniformFactory.i
                      UserDefined.i
                      UserDefinedPair.i
                      UserDefinedFactory.i
                      Weibull.i
                      WeibullFactory.i
                      ZipfMandelbrot.i
                      UncertaintyDistTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                      
ot_add_python_module( bayesian bayesian_module.i
                      CalibrationStrategyImplementation.i
                      CalibrationStrategy.i
                      SamplerImplementation.i
                      Sampler.i
                      MCMC.i
                      RandomWalkMetropolisHastings.i
                      PosteriorRandomVector.i
                      UncertaintyBayesianTemplateDefs.i
                      PythonWrappingFunctions.hxx
                     )           
                      
ot_add_python_module( classification classification_module.i
                      MixtureClassifier.i
                      UncertaintyClassificationTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( weightedexperiment weightedexperiment_module.i
                      WeightedExperiment.i
                      BootstrapExperiment.i
                      FixedExperiment.i
                      ImportanceSamplingExperiment.i
                      GaussProductExperiment.i
                      LHSExperiment.i
                      LowDiscrepancyExperiment.i
                      MonteCarloExperiment.i
                      UncertaintyWeightedExperimentTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( orthogonalbasis orthogonalbasis_module.i
                      EnumerateFunction.i
                      EnumerateFunctionImplementation.i
                      LinearEnumerateFunction.i
                      HyperbolicAnisotropicEnumerateFunction.i
                      OrthogonalUniVariatePolynomial.i
                      OrthogonalUniVariatePolynomialFactory.i
                      CharlierFactory.i
                      HermiteFactory.i
                      JacobiFactory.i
                      KrawtchoukFactory.i
                      LaguerreFactory.i
                      LegendreFactory.i
                      MeixnerFactory.i
                      OrthogonalUniVariatePolynomialFamily.i
                      OrthonormalizationAlgorithmImplementation.i
                      OrthonormalizationAlgorithm.i
                      ChebychevAlgorithm.i
                      GramSchmidtAlgorithm.i
                      StandardDistributionPolynomialFactory.i
                      OrthogonalFunctionFactory.i
                      OrthogonalProductPolynomialFactory.i
                      OrthogonalBasis.i
                      UncertaintyOrthogonalBasisTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( metamodel metamodel_module.i
                      MetaModelResult.i
                      MetaModelAlgorithm.i
                      LinearTaylor.i
                      QuadraticTaylor.i
                      LinearLeastSquares.i
                      QuadraticLeastSquares.i
                      AdaptiveStrategyImplementation.i
                      FixedStrategy.i
                      SequentialStrategy.i
                      CleaningStrategy.i
                      AdaptiveStrategy.i
                      ProjectionStrategyImplementation.i
                      LeastSquaresStrategy.i
                      IntegrationStrategy.i
                      ProjectionStrategy.i
                      FunctionalChaosResult.i
                      FunctionalChaosAlgorithm.i
                      KrigingAlgorithm.i
                      RandomVector.i
                      PythonNumericalMathEvaluationImplementation.cxx
                      PythonRandomVectorImplementation.cxx
                      CompositeRandomVector.i
                      EventRandomVectorImplementation.i
                      EventDomainImplementation.i
                      EventProcess.i
                      ConditionalRandomVector.i
                      Event.i
                      StandardEvent.i
                      FunctionalChaosRandomVector.i
                      UncertaintyMetaModelTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                     
ot_add_python_module( transformation transformation_module.i
                      MarginalTransformationEvaluation.i
                      MarginalTransformationGradient.i
                      MarginalTransformationHessian.i
                      NatafEllipticalCopulaEvaluation.i
                      NatafEllipticalCopulaGradient.i
                      NatafEllipticalCopulaHessian.i
                      InverseNatafEllipticalCopulaEvaluation.i
                      InverseNatafEllipticalCopulaGradient.i
                      InverseNatafEllipticalCopulaHessian.i
                      NatafIndependentCopulaEvaluation.i
                      NatafIndependentCopulaGradient.i
                      NatafIndependentCopulaHessian.i
                      InverseNatafIndependentCopulaEvaluation.i
                      InverseNatafIndependentCopulaGradient.i
                      InverseNatafIndependentCopulaHessian.i
                      NatafEllipticalDistributionEvaluation.i
                      NatafEllipticalDistributionGradient.i
                      NatafEllipticalDistributionHessian.i
                      InverseNatafEllipticalDistributionEvaluation.i
                      InverseNatafEllipticalDistributionGradient.i
                      InverseNatafEllipticalDistributionHessian.i
                      RosenblattEvaluation.i
                      InverseRosenblattEvaluation.i
                      BoxCoxFactory.i
                      TrendFactory.i
                      UncertaintyTransformationTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )

ot_add_python_module( analytical analytical_module.i
                      AnalyticalResult.i
                      Analytical.i
                      FORMResult.i
                      FORM.i
                      SORMResult.i
                      SORM.i
                      StrongMaximumTest.i
                      UncertaintyAnalyticalTemplateDefs.i
                      PythonWrappingFunctions.hxx
                    )
                     
ot_add_python_module( simulation simulation_module.i
                      SimulationSensitivityAnalysis.i
                      SimulationResultImplementation.i
                      SimulationResult.i
                      Simulation.i
                      PostAnalyticalSimulation.i
                      Wilks.i
                      MonteCarlo.i
                      LHS.i
                      RandomizedLHS.i
                      ImportanceSampling.i
                      PostAnalyticalControlledImportanceSampling.i
                      PostAnalyticalImportanceSampling.i
                      RootStrategyImplementation.i
                      RootStrategy.i
                      SamplingStrategyImplementation.i
                      SamplingStrategy.i
                      DirectionalSampling.i
                      OrthogonalDirection.i
                      RandomDirection.i
                      MediumSafe.i
                      RiskyAndFast.i
                      SafeAndSlow.i
                      QuasiMonteCarlo.i
                      RandomizedQuasiMonteCarlo.i
                      QuasiMonteCarloResult.i
                      UncertaintySimulationTemplateDefs.i
                      PythonWrappingFunctions.hxx
                     )
                     
ot_add_python_module( stattests stattests_module.i
                      VisualTest.i
                      FittingTest.i
                      HypothesisTest.i
                      LinearModelTest.i
                      NormalityTest.i
                      DickeyFullerTest.i
                      UncertaintyStatTestsTemplateDefs.i
                      PythonWrappingFunctions.hxx
                     )
                     
ot_add_python_module( uncertainty uncertainty_module.i
                      QuadraticCumul.i
                      UncertaintyTemplateDefs.i
                      PythonWrappingFunctions.hxx
                     )
                     
ot_add_python_module( testing testing_module.i
                     )
                     
foreach ( module ${OPENTURNS_PYTHON_MODULES} )
  install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${module}.py
            DESTINATION ${OPENTURNS_PYTHON_MODULE_PATH}/openturns
            )
  install ( TARGETS _${module}
            LIBRARY DESTINATION ${OPENTURNS_PYTHON_MODULE_PATH}/openturns
          )
endforeach ()

install ( FILES __init__.py
                dist.py
                test.py
                viewer.py
                coupling_tools.py
          DESTINATION ${OPENTURNS_PYTHON_MODULE_PATH}/openturns
        )

install ( FILES ${SWIGFILES}
          DESTINATION ${OPENTURNS_INCLUDE_PATH}/swig
        )

        
