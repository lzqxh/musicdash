#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>

namespace Message {
	const std::string next_timeslice	= "next_timeslice";

	const std::string input_slide_left	= "input_slide_left";
	const std::string input_slide_right	= "input_slide_right";
	const std::string input_slide_up	= "input_slide_up";
	const std::string input_click	= "input_click";
	const std::string input_touch_release		= "input_touch_release";

	const std::string score = "score";

	const std::string disp_effect = "disp_effect";
	const std::string explode = "explode";
};


#endif //__MESSAGE_H__