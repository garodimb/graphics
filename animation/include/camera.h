#ifndef _CAMERA_H
#define _CAMERA_H

#include <quaternion.h>
#include <datatypes.h>
#include <log.h>

class Camera{
	private:
		Vector fix_pos;
		Vector curr_pos;
		Vector fix_up;
		Vector curr_up;
		Vector fix_lookat;
		Vector curr_lookat;

	public:
		Camera(Vector& position, Vector& lookat, Vector &up);
		~Camera();
		int set_camera();
		int config_camera(Vector& position, Vector &lookat, Vector &up);
		Vector get_position();
		Vector get_lookat();
		Vector get_up();
		int rotate_camera(Quaternion q);
};

#endif