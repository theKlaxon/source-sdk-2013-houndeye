//
// Created by ENDERZOMBI102 on 26/10/2023.
//

#pragma once


struct ButtonEntry_t {
	const char* name;
	const ButtonCode_t code;
};

extern std::array<ButtonEntry_t, 107> BUTTON_MAP;
