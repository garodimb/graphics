#include <iostream>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>
#include <view.h>

View *view;

static void on_display(){
	view->display();
	}

static void on_reshape(int w,int h){
	view->reshape(w,h);
	}

static void on_idle_func_handler()
{
	view->idle_func_handler();
}

/* Constructor */
View::View(int argc,char **argv){
	track_matrix= NULL; // Trackball Matrix
	rotate_x 	= 0.0f; // X Rotation
	rotate_y 	= 0.0f; // Y Rotation
	z_distance 	= 0.0f; // Camera distance relative to current position
	scale_all	= 1.0f; // Uniform scaling in all direction
	trans_x  	= 0.0f; // X translation
	trans_y  	= 0.0f; // Y translation
	trans_z  	= 0.0f; // Z translation
	light_status[0] = light_status[1] = light_status[2] = light_status[3] = true;
	light_status[4] = true;
	glutInit(&argc,argv);
	init_window("Texture Mapping",1000,500);
	init();
	init_camera();
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutIdleFunc(on_idle_func_handler);
	view = this;
	cube = new Cube(4.0f);
	init_scene(argc,argv);
	}

/* Desctructor */
View::~View()
{
	for(int i = 0;i<num_models;i++)
		delete model[i];
	delete [] model;
	delete camera;
	delete scene;
}

/* Initialize windowing system */
int View::init_window(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	glutInitWindowPosition(pos[0],pos[1]);
	glutCreateWindow(title);
	return 0;
	}

/* Init canvas */

int View::init_lighting(){

	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_diffuse[] = {0.8,0.8,0.8,1.0};
	GLfloat mat_shininess[] = { 128.0 };

	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_diffuse1[] = { 1.0, 0.0, 1.0, 0.0 };
	GLfloat light_diffuse2[] = { 0.0, 1.0, 1.0, 0.0 };
	GLfloat qaDiffuseLight[] = {1, 0, 1, 1.0};

	/* Disable LIGHTING to view only colored cube with different colors*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, qaDiffuseLight);
	return 0;
	}

/*
 * Initialize camera position and vector
 */
int View::init_camera(void)
{
	Vector cam_pos,cam_up,cam_lookat;
	cam_pos.x = 0.0f;
	cam_pos.y = -1.0f;
	cam_pos.z = -4.0f;

	cam_up.x = cam_up.z = 0.0f;
	cam_up.y = 1.0;

	cam_lookat.x = cam_lookat.z = 0.0f;
	cam_lookat.y = -1.0f;
	delete camera;
	camera = new Camera(cam_pos,cam_lookat,cam_up);
	return 0;
}

int View::init(void){

	glClearColor(0.0,0.0,0.0,0.0);
	glClearStencil(0);
	glEnable(GL_NORMALIZE); //Dont care about performance now
	glShadeModel (GL_SMOOTH);
	init_lighting();
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	return 0;
	}

/*
 * Initialize scene
 */
int View::init_scene(int argc,char **argv){
	string fn1_obj,fn2_obj,tex1_path,tex2_path;
	float matrix[16];
	Matrix mat;
	num_models = 2;
	model = new Model*[num_models];
	node = new SceneNode*[num_nodes];
	scene = new SceneNode();
	fn1_obj = "plyfiles/canstick.ply";
	fn2_obj = "plyfiles/apple.ply";
	tex1_path = "texfiles/canstick.bmp";
	tex2_path = "texfiles/apple.bmp";
	if(argc>=2){
		fn1_obj = argv[1];
		}
	if(argc>=3){
		fn2_obj = argv[2];
		}
	if(argc>=4){
		tex1_path = argv[3];
		}
	if(argc>=5){
		tex1_path = argv[4];
		}

	 /* Create Models */
	model[0] = new Model(fn1_obj,Cylinder,tex1_path);
	model[1] = new Model(fn2_obj,Sphere,tex2_path);
	node[0] = new SceneNode();
	node[1] = new SceneNode();

	node[0]->set_model(model[0]);
	mat.get_Tmat(0.8,0,0,matrix);
	node[1]->set_transf(matrix);

	mat.get_Tmat(-0.8,0,0,matrix);
	node[0]->set_transf(matrix);
	node[1]->set_model(model[1]);

	scene->add_child(node[0]);
	scene->add_child(node[1]);
	return 0;
}
int View::get_width(){

	return screen_width;
	}

int View::get_height(){

	return screen_height;
	}

/* Reshape Handler */
void View::reshape(int w,int h){
	log_D("Handling reshape");
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, (float)w/h, 0.1f,15.0f);
	log_D("Window Width: "<<w<<", Height: "<<h);
	screen_height=h;
	screen_width=w;
	}

/* Display Handler */
void View::display(){
	log_D("Displaying content");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	set_headlight();
	set_camera();
	set_fixed_light();
	glScalef(scale_all,scale_all,scale_all);
	/* Move camera using z so to get fill of zooming */
	/* Eye, Center and Up vector */
	glStencilFunc(GL_ALWAYS,1,-1);
	cube->display();
	scene->display();
	glutSwapBuffers();
	}

/* Idle function handler */
void View::idle_func_handler(void)
{
	static float count=0.0f;
	static int flag = 0;
	Matrix mat;
	float trans_mat[16];
	mat.get_Tmat(count,0,0,trans_mat);
	double x;
	trans_mat[12] = modf(trans_mat[12],&x)*4;
	trans_mat[14] = sinf(count*20)*2;
	if(trans_mat[12]>=2){
		flag=1;
		}
	if(trans_mat[12]<=-2){
		flag=0;
		}
	if(!flag)
		count += 0.001;
	else
		count -= 0.001;
	node[0]->set_transf(trans_mat);
	usleep(30000);
	glutPostRedisplay();
}

/* Draw Reference axis */
int View::draw_axis(){
	log_D("Drawing axis");
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(-1.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);

		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,-1.0,0.0);
		glVertex3f(0.0,1.0,0.0);

		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();
	return 0;
	}

/* Rotate using Quternion, Arraow Rotation */
int View::rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
	Quaternion quaternion;
	quaternion.CreateFromAxisAngle(x,y,z,angle);
	camera->rotate_camera(quaternion);
	return 0;
	}

/* Refresh view */
int View::refresh(GLfloat rotate_x,GLfloat rotate_y,GLfloat z_distance,GLfloat scale_all,GLfloat trans_x,GLfloat trans_y,GLfloat trans_z,GLfloat *track_matrix,bool *light_status){
			this->rotate_x 	= rotate_x; // X Rotation
			this->rotate_y 	= rotate_y; // Y Rotation
			this->z_distance = z_distance; // Camera distance
			this->scale_all	= scale_all; // Uniform scaling in all direction
			this->track_matrix = track_matrix; //Rotation by Trackball
			for(int i =0 ;i<5;i++)
				this->light_status[i] = light_status[i];
			glutPostRedisplay();
			return 0;
			}

/* Mouse Handler registration */
int View::reg_mouse_handler(void (*mouse_handler)(int button,int state,int x,int y)){
	if(mouse_handler==NULL){
		return 1;
		}
	glutMouseFunc(mouse_handler);
	return 0;
	}

/* Motion Handler registration */
int View::reg_motion_handler(void (*motion_handler)(int x,int y)){
	if(motion_handler==NULL){
		return 1;
		}
	glutMotionFunc(motion_handler);
	return 0;
	}

/* Keyboard Handler registration */
int View::reg_keyboard_handler(void (*keyboard_handler)(unsigned char key,int x,int y)){
	if(keyboard_handler==NULL){
		return 1;
		}
	glutKeyboardFunc(keyboard_handler);
	return 0;
	}

/* Keyboard Special Handler registration */
int View::reg_keyboard_spec_handler(void (*keyboard_spec_handler)(int key,int x,int y)){
	if(keyboard_spec_handler==NULL){
		return 1;
		}
	glutSpecialFunc(keyboard_spec_handler);
	return 0;
	}

int View::set_camera()
{
	Quaternion q;
	Vector curr_pos;
	if(track_matrix){
		q.x = track_matrix[0];
		q.y = track_matrix[1];
		q.z = track_matrix[2];
		q.w = track_matrix[3];
		}
	~q;
	camera->rotate_camera(q);
	curr_pos = camera->get_position();
	gluLookAt(curr_pos.x, curr_pos.y,curr_pos.z, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);
	return 0;
}

/*
 * Set position for fixed light
 * To make light fix call it after camera setup
 */
int View::set_fixed_light()
{
	GLfloat light_position0[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat light_position1[] = { 0.0, 0.0, -3.0, 1.0 };
	GLfloat light_position2[] = { -3.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

	GLfloat direction[] = {0,2.0,0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	/* check which light to enable and which to disable */
	if(light_status[0])
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	if(light_status[1])
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
	if(light_status[2])
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
	if(light_status[4])
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	return 0;
}

/*
 * Setup moving light(headlight)
 */
int View::set_headlight()
{
	GLfloat qaLightPosition[] = {0.0, 0.0, 3.0, 1};
	GLfloat dirVector0[] = { 0.3, -0.50, -4.0, 0.0};

	glLightfv(GL_LIGHT3, GL_POSITION, qaLightPosition);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 5.0);// set cutoff angle
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dirVector0);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 1); // set focusing strength
    if(light_status[3])
		glEnable(GL_LIGHT3);
	else
		glDisable(GL_LIGHT3);
	return 0;
}

/*
 * Chage texture of selected object
 */
int View::update_tex(string &tex_path,int obj)
{
	/*
	 * Choose object and update texture
	 */
	SceneNode * node = NULL;
	Model * mdl = NULL;
	if(obj==1){
		cube->update_tex(tex_path);
	}
	else{
		node = scene->get_scenenode(obj);
		if(!node){
			return 1;
			}
		mdl = node->get_model();
		if(!mdl){
			return 2;
			}
		mdl->update_tex(tex_path);
		}
	glutPostRedisplay();
	return 0;
}

/*
 * Change texture mode of selected model(OpenGL/Manual)
 */
int View::update_tex_mode(int mode,int obj)
{
	SceneNode * node = NULL;
	Model * mdl = NULL;
	node = scene->get_scenenode(obj);
	if(!node){
		return 1;
	}
	mdl = node->get_model();
	if(!mdl){
		return 2;
		}
	mdl->update_tex_mode(mode);
	glutPostRedisplay();
	return 0;
}

/*
 * Translate current selected object by given distance
 * along floor
 */
int View::update_trans_obj(float trans_x,float trans_z,int curr_obj)
{
		SceneNode * node = NULL;
	Model * mdl = NULL;
	node = scene->get_scenenode(curr_obj);
	if(!node){
		return 1;
	}
	mdl = node->get_model();
	if(!mdl){
		return 2;
		}
	mdl->trans_obj_by(trans_x,trans_z);
	glutPostRedisplay();
	return 0;
}