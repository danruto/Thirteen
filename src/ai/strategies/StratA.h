#pragma once
#ifndef STRATA_H
#define STRATA_H

#include "Strategy.h"


class StratA : public Strategy
{
public:
	std::vector<int> cardsToPlay(std::vector<Card> hand, std::vector<Card> lastPlayed);
};

#endif