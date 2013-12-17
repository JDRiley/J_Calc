#ifndef J_PXC_FACE_H
#define J_PXC_FACE_H

//
#include "J_PXC_Fwd_Decl.h"
//
#include "pxcface.h"

#include <string>
//
#include <array>

J_POINTER_DECL(PXCFaceAnalysis, PXC_Face_Module)


namespace jomike{


typedef PXCFaceAnalysis::Detection::Data PXC_Face_Data;
typedef PXCFaceAnalysis::Landmark* PXC_Face_Landmark;
typedef PXCFaceAnalysis::Detection* PXC_Face_Detection;

typedef PXCFaceAnalysis::Landmark::PoseData	 PXC_Face_Pose_Data;


typedef std::array<PXC_Face_Data, MAX_FACES> Face_Data_Cont_t;



typedef PXCFaceAnalysis::Landmark::LandmarkData PXC_Face_Landmark_Data;


typedef ex_array<ex_array<PXC_Face_Landmark_Data>> Landmark_Data_Cont_t;

}

#endif