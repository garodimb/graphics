#include <iostream>
#include <cstring>
#include <matrix.h>
#include <math.h>

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}
/*
 * Get identity matrix of 4X4
 */
int Matrix::get_Imat(float *mat)
{
	memset(mat,0,MAT_ENT*sizeof(float));
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
	return 0;
}

/*
 * Get translation matrix of 4X4
 */
int Matrix::get_Tmat(float x, float y, float z,float *mat)
{
	get_Imat(mat);
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
	return 0;
}

/*
 * Get rotation matrix 4X4
 */
int Matrix::get_Rmat(float x,float y,float z,float angle_rad,float *mat)
{
	quat.CreateFromAxisAngle(x,y,z,angle_rad);
	quat.CreateMatrix(mat);
	return 0;
}

/*
 * Get Scaling matrix of 4X4
 */
int Matrix::get_Smat(float x, float y, float z,float *mat)
{
	get_Imat(mat);
	mat[0] = x;
	mat[5] = y;
	mat[10] = z;
	return 0;
}


float Matrix::get_angle(const Vector &vec1,const Vector &vec2)
{
	float angle = 0.0;
	float v1_mag = sqrt(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z * vec1.z);
	float v2_mag = sqrt(vec2.x*vec2.x + vec2.y*vec2.y + vec2.z * vec2.z);
	float value = (vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z)/(v1_mag*v2_mag);
	/* Handle corner cases where value might go sligtly outside range */
	if(value>1.0)
		value = 1.0f;
	if(value<-1.0)
		value = -1.0f;
	angle = acos(value);
	angle = angle * 180 / M_PI;
	log_D("Angle: "<<angle);
	return angle;
}
/*
 * MAT_C = MAT_A * MAT_B
 */
int Matrix::mul_mat(float *mata,float *matb,float *matc)
{
	int n = 4;
	float ans[MAT_ENT],sum=0;
	int i,j,k;
	for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        for (k = 0; k < n; k++) {
          sum = sum + mata[k*n+j]*matb[i*n+k];
        }
        ans[j+i*n] = sum;
        sum = 0;
      }
    }
    memcpy(matc,ans,MAT_ENT*sizeof(ans[0]));
    return 0;
}

int Matrix::get_transf_vector(float *mat,Vector& vec)
{
	Vector sol;
	sol.x = mat[0]*vec.x+mat[4]*vec.y+mat[8]*vec.z+mat[12];
	sol.y = mat[1]*vec.x+mat[5]*vec.y+mat[9]*vec.z+mat[13];
	sol.z = mat[2]*vec.x+mat[6]*vec.y+mat[10]*vec.z+mat[14];
	vec = sol;
	return 0;
}
/* Assume all are 4*4 matrices */
int Matrix::print_mat(float *mat)
{
	cout<<"**************************************"<<endl;
	for(int i=0;i<16;i++){
		cout<<mat[i]<<"  ";
		if((i+1)%4==0)
			cout<<endl;
		}
	cout<<"**************************************"<<endl;

}