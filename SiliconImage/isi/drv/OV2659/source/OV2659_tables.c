#include <ebase/types.h>
#include <ebase/trace.h>
#include <ebase/builtins.h>

#include <common/return_codes.h>

#include "isi.h"
#include "isi_iss.h"
#include "isi_priv.h"
#include "OV2659_priv.h"


/*****************************************************************************
 * DEFINES
 *****************************************************************************/


/*****************************************************************************
 * GLOBALS
 *****************************************************************************/

// Image sensor register settings default values taken from data sheet OV8810_DS_1.1_SiliconImage.pdf.
// The settings may be altered by the code in IsiSetupSensor.
const IsiRegDescription_t OV2659_g_aRegDescription[] =
{
	{0x3000, 0x0f,"",eReadWrite},
	{0x3001, 0xff,"",eReadWrite},
	{0x3002, 0xff,"",eReadWrite},

	{0x3633, 0x3d,"",eReadWrite},
	{0x3620, 0x02,"",eReadWrite},
	{0x3631, 0x11,"",eReadWrite},
	{0x3612, 0x04,"",eReadWrite},
	{0x3630, 0x20,"",eReadWrite},
	{0x4702, 0x02,"",eReadWrite},
	{0x370c, 0x34,"",eReadWrite},
	{0x3004, 0x10,"",eReadWrite},
	{0x3005, 0x18,"",eReadWrite},
	{0x3800, 0x00,"",eReadWrite},
	{0x3801, 0x00,"",eReadWrite},
	{0x3802, 0x00,"",eReadWrite},
	{0x3803, 0x00,"",eReadWrite},
	{0x3804, 0x06,"",eReadWrite},
	{0x3805, 0x5f,"",eReadWrite},
	{0x3806, 0x04,"",eReadWrite},
	{0x3807, 0xb7,"",eReadWrite},
	{0x3808, 0x03,"",eReadWrite},
	{0x3809, 0x20,"",eReadWrite},
	{0x380a, 0x02,"",eReadWrite},
	{0x380b, 0x58,"",eReadWrite},
	{0x380c, 0x05,"",eReadWrite},
	{0x380d, 0x14,"",eReadWrite},
	{0x380e, 0x02,"",eReadWrite},
	{0x380f, 0x68,"",eReadWrite},
	{0x3811, 0x08,"",eReadWrite},
	{0x3813, 0x02,"",eReadWrite},
	{0x3814, 0x31,"",eReadWrite},
	{0x3815, 0x31,"",eReadWrite},
	{0x3a02, 0x02,"",eReadWrite},
	{0x3a03, 0x68,"",eReadWrite},
	{0x3a08, 0x00,"",eReadWrite},
	{0x3a09, 0x5c,"",eReadWrite},
	{0x3a0a, 0x00,"",eReadWrite},
	{0x3a0b, 0x4d,"",eReadWrite},
	{0x3a0d, 0x08,"",eReadWrite},
	{0x3a0e, 0x06,"",eReadWrite},
	{0x3a14, 0x02,"",eReadWrite},
	{0x3a15, 0x28,"",eReadWrite},
	{0x4708, 0x01,"",eReadWrite},
	{0x3623, 0x00,"",eReadWrite},
	{0x3634, 0x76,"",eReadWrite},
	{0x3701, 0x44,"",eReadWrite},
	{0x3702, 0x18,"",eReadWrite},
	{0x3703, 0x24,"",eReadWrite},
	{0x3704, 0x24,"",eReadWrite},
	{0x3705, 0x0c,"",eReadWrite},
	{0x3820, 0x81,"",eReadWrite},
	{0x3821, 0x01,"",eReadWrite},
	{0x370a, 0x52,"",eReadWrite},
	{0x4608, 0x00,"",eReadWrite},
	{0x4609, 0x80,"",eReadWrite},
	{0x4300, 0x32,"",eReadWrite}, //0x32:uyvy,0x30,yuyv
	{0x5086, 0x02,"",eReadWrite},
	{0x5000, 0xfb,"",eReadWrite},
	{0x5001, 0x1f,"",eReadWrite},
	{0x5002, 0x00,"",eReadWrite},
	{0x5025, 0x0e,"",eReadWrite},
	{0x5026, 0x18,"",eReadWrite},
	{0x5027, 0x34,"",eReadWrite},
	{0x5028, 0x4c,"",eReadWrite},
	{0x5029, 0x62,"",eReadWrite},
	{0x502a, 0x74,"",eReadWrite},
	{0x502b, 0x85,"",eReadWrite},
	{0x502c, 0x92,"",eReadWrite},
	{0x502d, 0x9e,"",eReadWrite},
	{0x502e, 0xb2,"",eReadWrite},
	{0x502f, 0xc0,"",eReadWrite},
	{0x5030, 0xcc,"",eReadWrite},
	{0x5031, 0xe0,"",eReadWrite},
	{0x5032, 0xee,"",eReadWrite},
	{0x5033, 0xf6,"",eReadWrite},
	{0x5034, 0x11,"",eReadWrite},
	{0x5070, 0x1c,"",eReadWrite},
	{0x5071, 0x5b,"",eReadWrite},
	{0x5072, 0x05,"",eReadWrite},
	{0x5073, 0x20,"",eReadWrite},
	{0x5074, 0x94,"",eReadWrite},
	{0x5075, 0xb4,"",eReadWrite},
	{0x5076, 0xb4,"",eReadWrite},
	{0x5077, 0xaf,"",eReadWrite},
	{0x5078, 0x05,"",eReadWrite},
	{0x5079, 0x98,"",eReadWrite},
	{0x507a, 0x21,"",eReadWrite},
	{0x5035, 0x6a,"",eReadWrite},
	{0x5036, 0x11,"",eReadWrite},
	{0x5037, 0x92,"",eReadWrite},
	{0x5038, 0x21,"",eReadWrite},
	
	{0x5039, 0xe1,"",eReadWrite},
	{0x503a, 0x01,"",eReadWrite},
	{0x503c, 0x05,"",eReadWrite},
	{0x503d, 0x08,"",eReadWrite},
	{0x503e, 0x08,"",eReadWrite},
	{0x503f, 0x64,"",eReadWrite},
	{0x5040, 0x58,"",eReadWrite},
	{0x5041, 0x2a,"",eReadWrite},
	{0x5042, 0xc5,"",eReadWrite},
	{0x5043, 0x2e,"",eReadWrite},
	{0x5044, 0x3a,"",eReadWrite},
	{0x5045, 0x3c,"",eReadWrite},
	{0x5046, 0x44,"",eReadWrite},
	{0x5047, 0xf8,"",eReadWrite},
	{0x5048, 0x08,"",eReadWrite},
	{0x5049, 0x70,"",eReadWrite},
	{0x504a, 0xf0,"",eReadWrite},
	{0x504b, 0xf0,"",eReadWrite},
	{0x500c, 0x03,"",eReadWrite},
	{0x500d, 0x20,"",eReadWrite},
	{0x500e, 0x02,"",eReadWrite},
	{0x500f, 0x5c,"",eReadWrite},
	{0x5010, 0x48,"",eReadWrite},
	{0x5011, 0x00,"",eReadWrite},
	{0x5012, 0x66,"",eReadWrite},
	{0x5013, 0x03,"",eReadWrite},
	{0x5014, 0x30,"",eReadWrite},
	{0x5015, 0x02,"",eReadWrite},
	{0x5016, 0x7c,"",eReadWrite},
	{0x5017, 0x40,"",eReadWrite},
	{0x5018, 0x00,"",eReadWrite},
	{0x5019, 0x66,"",eReadWrite},
	{0x501a, 0x03,"",eReadWrite},
	{0x501b, 0x10,"",eReadWrite},
	{0x501c, 0x02,"",eReadWrite},
	{0x501d, 0x7c,"",eReadWrite},
	{0x501e, 0x3a,"",eReadWrite},
	{0x501f, 0x00,"",eReadWrite},
	{0x5020, 0x66,"",eReadWrite},
	{0x506e, 0x44,"",eReadWrite},
	{0x5064, 0x08,"",eReadWrite},
	{0x5065, 0x10,"",eReadWrite},
	{0x5066, 0x12,"",eReadWrite},
	{0x5067, 0x02,"",eReadWrite},
	{0x506c, 0x08,"",eReadWrite},
	{0x506d, 0x10,"",eReadWrite},
	{0x506f, 0xa6,"",eReadWrite},
	{0x5068, 0x08,"",eReadWrite},
	
	
	{0x5069, 0x10,"",eReadWrite},
	{0x506a, 0x04,"",eReadWrite},
	{0x506b, 0x12,"",eReadWrite},
	{0x507e, 0x40,"",eReadWrite},
	{0x507f, 0x20,"",eReadWrite},
	{0x507b, 0x02,"",eReadWrite},
	{0x507a, 0x01,"",eReadWrite},
	{0x5084, 0x0c,"",eReadWrite},
	{0x5085, 0x3e,"",eReadWrite},
	{0x5005, 0x80,"",eReadWrite},
	{0x3a0f, 0x30,"",eReadWrite},
	{0x3a10, 0x28,"",eReadWrite},
	{0x3a1b, 0x32,"",eReadWrite},
	{0x3a1e, 0x26,"",eReadWrite},
	{0x3a11, 0x60,"",eReadWrite},
	{0x3a1f, 0x14,"",eReadWrite},
	{0x5060, 0x69,"",eReadWrite},
	{0x5061, 0x7d,"",eReadWrite},
	{0x5062, 0x7d,"",eReadWrite},
	{0x5063, 0x69,"",eReadWrite},
	{0x3004, 0x20,"",eReadWrite},
	{0x0100, 0x01,"",eReadWrite},
    {0x0000 ,0x00,"eTableEnd",eTableEnd}

};

const IsiRegDescription_t OV2659_g_svga[] =
{

	{0x0100, 0x00,"",eReadWrite},    //software sleep : Sensor vsync singal may not output if haven't sleep the sensor when transfer the array,
	{0x3800, 0x00,"",eReadWrite},
	{0x3801, 0x00,"",eReadWrite},
	{0x3802, 0x00,"",eReadWrite},
	{0x3803, 0x00,"",eReadWrite},
	{0x3804, 0x06,"",eReadWrite},
	{0x3805, 0x5f,"",eReadWrite},
	{0x3806, 0x04,"",eReadWrite},
	{0x3807, 0xb7,"",eReadWrite},
	{0x3808, 0x03,"",eReadWrite},
	{0x3809, 0x20,"",eReadWrite},
	{0x380a, 0x02,"",eReadWrite},
	{0x380b, 0x58,"",eReadWrite},
	{0x380c, 0x05,"",eReadWrite},
	{0x380d, 0x14,"",eReadWrite},
	{0x380e, 0x02,"",eReadWrite},
	{0x380f, 0x68,"",eReadWrite},
	{0x3811, 0x08,"",eReadWrite},
	{0x3813, 0x02,"",eReadWrite},
	{0x3814, 0x31,"",eReadWrite},
	{0x3815, 0x31,"",eReadWrite},
	{0x3a02, 0x02,"",eReadWrite},
	{0x3a03, 0x68,"",eReadWrite},
	{0x3a08, 0x00,"",eReadWrite},
	{0x3a09, 0x5c,"",eReadWrite},
	{0x3a0a, 0x00,"",eReadWrite},
	{0x3a0b, 0x4d,"",eReadWrite},
	{0x3a0d, 0x08,"",eReadWrite},
	{0x3a0e, 0x06,"",eReadWrite},
	{0x3a14, 0x02,"",eReadWrite},
	{0x3a15, 0x28,"",eReadWrite},
	{0x3623, 0x00,"",eReadWrite},
	{0x3634, 0x76,"",eReadWrite},
	{0x3701, 0x44,"",eReadWrite},
	{0x3702, 0x18,"",eReadWrite},
	{0x3703, 0x24,"",eReadWrite},
	{0x3704, 0x24,"",eReadWrite},
	{0x3705, 0x0c,"",eReadWrite},
	{0x3820, 0x81,"",eReadWrite},
	{0x3821, 0x01,"",eReadWrite},
	{0x370a, 0x52,"",eReadWrite},
	{0x4608, 0x00,"",eReadWrite},
	{0x4609, 0x80,"",eReadWrite},
	{0x5002, 0x10,"",eReadWrite},
	{0x3005, 0x18,"",eReadWrite},
	{0x3004, 0x20,"",eReadWrite},
	{0x3503,0x00,"",eReadWrite},
	{0x0100, 0x01,"",eReadWrite}, 	//software wake
    {0x0000 ,0x00,"eTableEnd",eTableEnd}
};

const IsiRegDescription_t OV2659_g_1600x1200[] =
{
	{0x3503,0x03,"",eReadWrite}, 
	{0x506e,0x44,"",eReadWrite},	
	{0x5064,0x08,"",eReadWrite},	
	{0x5065,0x10,"",eReadWrite},
	{0x5066,0x18,"",eReadWrite},	// zenghaihui 20110920 16
	{0x5067,0x10,"",eReadWrite},
	{0x506c,0x08,"",eReadWrite},
	{0x506d,0x10,"",eReadWrite},	
	{0x506f,0xa6,"",eReadWrite},	
	{0x5068,0x08,"",eReadWrite},
	{0x5069,0x10,"",eReadWrite},	
	{0x506a,0x08,"",eReadWrite},
	{0x506b,0x28,"",eReadWrite},
	{0x5084,0x14,"",eReadWrite},//0c
	{0x5085,0x3c,"",eReadWrite},//34	
	{0x5005,0x80,"",eReadWrite}, 



	{0x5066, 0x3c,"",eReadWrite}, 		
	{0x5067, 0x1a,"",eReadWrite}, 
	{0x506a, 0x0e,"",eReadWrite},    
	{0x506b, 0x2e,"",eReadWrite},    

	{0x3800, 0x00,"",eReadWrite}, 
	{0x3801, 0x00,"",eReadWrite}, 
	{0x3802, 0x00,"",eReadWrite}, 
	{0x3803, 0x00,"",eReadWrite}, 
	{0x3804, 0x06,"",eReadWrite}, 
	{0x3805, 0x5f,"",eReadWrite}, 
	{0x3806, 0x04,"",eReadWrite}, 
	{0x3807, 0xbb,"",eReadWrite}, 
	{0x3808, 0x06,"",eReadWrite}, 
	{0x3809, 0x40,"",eReadWrite}, 
	{0x380a, 0x04,"",eReadWrite}, 
	{0x380b, 0xb0,"",eReadWrite}, 
	{0x3811, 0x10,"",eReadWrite}, 
	{0x3813, 0x06,"",eReadWrite}, 
	{0x3814, 0x11,"",eReadWrite}, 
	{0x3815, 0x11,"",eReadWrite}, 

	{0x3623, 0x00,"",eReadWrite}, 
	{0x3634, 0x44,"",eReadWrite}, 
	{0x3701, 0x44,"",eReadWrite}, 
	{0x3208, 0xa2,"",eReadWrite}, 
	{0x3705, 0x18,"",eReadWrite}, 	 
	{0x3820, 0x80,"",eReadWrite}, 
	{0x3821, 0x00,"",eReadWrite}, 

	{0x3003, 0x80,"",eReadWrite},//10fps 
	{0x3004, 0x20,"",eReadWrite}, //10		 
	{0x3005, 0x18,"",eReadWrite}, 
	{0x3006, 0x0d,"",eReadWrite}, 

	{0x380c, 0x07,"",eReadWrite}, 
	{0x380d, 0x9f,"",eReadWrite}, 
	{0x380e, 0x04,"",eReadWrite}, 
	{0x380f, 0xd0,"",eReadWrite}, 

	{0x370a, 0x12,"",eReadWrite}, 
	{0x4608, 0x00,"",eReadWrite}, 
	{0x4609, 0x80,"",eReadWrite}, 
	{0x5002, 0x00,"",eReadWrite}, 

	{0x3a08, 0x00,"",eReadWrite}, 
	{0x3a09, 0x3e,"",eReadWrite},//7b 
	{0x3a0e, 0x13,"",eReadWrite},//0a 

	{0x3a0a, 0x00,"",eReadWrite}, 
	{0x3a0b, 0x3e,"",eReadWrite},//7b 				
	{0x3a0d, 0x13,"",eReadWrite},//0a 	

	{0x4003, 0x88,"",eReadWrite},
    {0x0000 ,0x00,"eTableEnd",eTableEnd}

};


