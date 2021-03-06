
#ifndef _H_SCENE_NODE
#define _H_SCENE_NODE
#include <iostream>
#include <vector>
#include <algorithm>
#include <datatypes.h>
#include <model.h>
#include <matrix.h>
#include <log.h>

using namespace std;
class SceneNode	{
	public:
		SceneNode();
		~SceneNode();
		Model * get_model();
		int set_model(Model *model);
		int add_child(SceneNode *node);
		/*
		 * mat is pointer to array having 16 consecutive floats
		 * set_transf copies data from mat to transformation matrix
		 * update_transf, updates transformation matrix
		 */
		int set_world_transf(float *mat);
		int update_world_transf(float *mat);
		int get_world_transf(float *mat);
		int get_global_world_tansf(float *mat);

		int set_local_transf(float *mat);
		int update_local_transf(float *mat);
		int get_local_transf(float *mat);
		/*
		 * Detach method will detach itself from parent(parent=NULL)
		 * Detach method will call parent's detach_child and detach
		 * it from parent side also
		 */
		int detach();
		int detach_child(SceneNode *child);
		 /*
		  * Get pointer to scenenode with having scene_id as id
		  * in a substree rooted at this node
		  */
		SceneNode * get_scenenode(int id);
		int display();

	protected:
		static int obj_count; //Current count for number of objects used to give id to each scenenode
		int scene_id;
		float *local_trans_mat;
		float *world_trans_mat;
		Model *model;
		SceneNode *parent;
		std :: vector < SceneNode *> child;
	};

#endif