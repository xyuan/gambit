//
// $Source$
// $Revision$
// $Date$
//
// DESCRIPTION:
// Interface to mixed-strategy equilibrium enumeration algorithms
//

#ifndef ALGENUMMIXED_H
#define ALGENUMMIXED_H

#include "game/efg.h"
#include "game/behavsol.h"
#include "game/nfg.h"
#include "game/mixedsol.h"

bool EnumMixedNfg(wxWindow *, const EFSupport &, gList<BehavSolution> &);
bool EnumMixedNfg(wxWindow *, const NFSupport &, gList<MixedSolution> &);

#endif  // ALGENUMMIXED_H