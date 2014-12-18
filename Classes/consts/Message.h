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
	const std::string disp_score = "disp_score";

	const std::string disp_effect = "disp_effect";
	const std::string explode = "explode";

	const std::string game_pause = "game_pause";
	const std::string game_stop = "game_stop";
	const std::string game_restart = "game_restart";
	const std::string game_resume = "game_resume";

	const std::string get_beer = "get_beer";
};


#endif //__MESSAGE_H__