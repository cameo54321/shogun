/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2012 Heiko Strathmann, Sergey Lisitsyn
 *
 */

#ifndef CROSSVALIDATIONMULTICLASSSTORAGE_H_
#define CROSSVALIDATIONMULTICLASSSTORAGE_H_

#include <shogun/evaluation/CrossValidationOutput.h>
#include <shogun/evaluation/BinaryClassEvaluation.h>
#include <shogun/labels/MulticlassLabels.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/lib/DynamicObjectArray.h>

namespace shogun
{

class CMachine;
class CLabels;
class CEvaluation;

/** @brief Class for storing multiclass evaluation information in every fold of cross-validation.
 *
 * Be careful - can be very expensive memory-wise. 
 */
class CCrossValidationMulticlassStorage: public CCrossValidationOutput
{
public:

	/** constructor */
	CCrossValidationMulticlassStorage() : CCrossValidationOutput()
	{
		m_pred_labels = NULL;
		m_true_labels = NULL;
		m_num_classes = 0;
		m_binary_evaluations = new CDynamicObjectArray();
	}

	/** destructor */
	virtual ~CCrossValidationMulticlassStorage()
	{
		for (int32_t i=0; i<m_num_folds*m_num_runs*m_num_classes; i++)
		{
			m_fold_ROC_graphs[i].~SGMatrix<float64_t>();
		}
		SG_FREE(m_fold_ROC_graphs);
		for (int32_t i=0; i<m_num_folds*m_num_runs*m_num_classes; i++)
		{
			m_fold_PRC_graphs[i].~SGMatrix<float64_t>();
		}
		SG_FREE(m_fold_PRC_graphs);
		SG_UNREF(m_binary_evaluations);
	};

	/** returns ROC of 1-v-R in given fold and run
	 * 
	 * @param run run
	 * @param fold fold
	 * @param c class
	 * @return ROC of 'run' run, 'fold' fold and 'c' class
	 */
	SGMatrix<float64_t> get_fold_ROC(int32_t run, int32_t fold, int32_t c)
	{
		ASSERT(0<=run);
		ASSERT(run<m_num_runs);
		ASSERT(0<=fold);
		ASSERT(fold<m_num_folds);
		ASSERT(0<=c);
		ASSERT(c<m_num_classes);
		return m_fold_ROC_graphs[run*m_num_folds*m_num_classes+fold*m_num_classes+c];
	}
	
	/** returns PRC of 1-v-R in given fold and run
	 * 
	 * @param run run
	 * @param fold fold
	 * @param c class
	 * @return ROC of 'run' run, 'fold' fold and 'c' class
	 */
	SGMatrix<float64_t> get_fold_PRC(int32_t run, int32_t fold, int32_t c)
	{
		ASSERT(0<=run);
		ASSERT(run<m_num_runs);
		ASSERT(0<=fold);
		ASSERT(fold<m_num_folds);
		ASSERT(0<=c);
		ASSERT(c<m_num_classes);
		return m_fold_PRC_graphs[run*m_num_folds*m_num_classes+fold*m_num_classes+c];
	}

	/** appends a binary evaluation instance
	 *
	 * @param evaluation binary evaluation to add
	 */
	void append_binary_evaluation(CBinaryClassEvaluation* evaluation)
	{
		m_binary_evaluations->push_back(evaluation);
	}

	/** returns binary evalution appended before
	 * 
	 * @param idx
	 */
	CBinaryClassEvaluation* get_binary_evaluation(int32_t idx)
	{
		return (CBinaryClassEvaluation*)m_binary_evaluations->get_element_safe(idx);
	}

	/** returns evaluation result of 1-v-R in given fold and run
	 *
	 * @param run run
	 * @param fold fold
	 * @param c class
	 * @param e evaluation number
	 */
	float64_t get_fold_evaluation_result(int32_t run, int32_t fold, int32_t c, int32_t e)
	{
		ASSERT(0<=run);
		ASSERT(run<m_num_runs);
		ASSERT(0<=fold);
		ASSERT(fold<m_num_folds);
		ASSERT(0<=c);
		ASSERT(c<m_num_classes);
		ASSERT(0<=e);
		int32_t n_evals = m_binary_evaluations->get_num_elements();
		ASSERT(e<n_evals);
		return m_evaluations_results[run*m_num_folds*m_num_classes*n_evals+fold*m_num_classes*n_evals+c*n_evals+e];
	}

	/** post init */
	virtual void post_init();

	/** post update results */
	virtual void post_update_results();

	/** expose labels 
	 * @param labels labels to expose
	 */
	virtual void init_expose_labels(CLabels* labels);

	/** update test result
	 *
	 * @param results result labels for test/validation run
	 * @param prefix prefix for output
	 */
	virtual void update_test_result(CLabels* results,
			const char* prefix="");

	/** update test true result
	 *
	 * @param results ground truth labels for test/validation run
	 * @param prefix prefix for output
	 */
	virtual void update_test_true_result(CLabels* results,
			const char* prefix="");

	/** @return name of SG_SERIALIZABLE */
	virtual const char* get_name() const { return "CrossValidationMulticlassStorage"; }

protected:

	/** custom binary evaluators */
	CDynamicObjectArray* m_binary_evaluations;

	/** fold evaluation results */
	SGVector<float64_t> m_evaluations_results;

	/** fold ROC graphs */
	SGMatrix<float64_t>* m_fold_ROC_graphs; 
	
	/** fold PRC graphs */
	SGMatrix<float64_t>* m_fold_PRC_graphs; 

	/** predicted results */
	CMulticlassLabels* m_pred_labels;

	/** true labels */
	CMulticlassLabels* m_true_labels;

	/** number of classes */
	int32_t m_num_classes;

};

}

#endif /* CROSSVALIDATIONMULTICLASSSTORAGE_H_ */