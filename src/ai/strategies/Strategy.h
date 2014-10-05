#pragma once
#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "..\..\model\Card.h"

class Strategy
{
public:
	std::vector<int> cardsToPlay(std::vector<Card> hand, std::vector<Card> lastPlayed);
};

#endif