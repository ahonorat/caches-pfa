/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file coherence.h
 * \brief Initialize SMC structure, link from SMC to C -- Header
 * \author ~ahonorat
 * \version 1.7
 * \date 16th march 2014
 * \warning Although there is a compilation warning, all this functions must 
 * have struct coherence * in parameter.
 * \note State of the sate machine cannot be known directly, so we need a 
 * duplicated information, each line contains his sate (M,O,E,S,I,F) and
 * if it's dirty, in order to be used by the code not generated by SMC.
 *
 */


#ifndef COHERENCE_H
#define COHERENCE_H

#include "add_line_hierarchy.h"

/**
 * \brief Up the stats of a specific cache (node).   
 */
void coherence_up_stat(struct coherence *this, struct node *node, unsigned long entry, int stats_type);

/**
 * \brief Make the line invalid.   
 */
void coherence_invalid_line(struct coherence *this, struct line *line);

/**
 * \brief Make the line modified.   
 */
void coherence_modify_line(struct coherence *this, struct line *line);

/**
 * \brief Make the line sahred.   
 */
void coherence_share_line(struct coherence *this, struct line *line);

/**
 * \brief Make the line exclusive.   
 */
void coherence_exclusive_line(struct coherence *this, struct line *line);

/**
 * \brief Make the line forwarder.   
 */
void coherence_forward_line(struct coherence *this, struct line *line);

/**
 * \brief Make the line owned.   
 */
void coherence_owned_line(struct coherence *this, struct line *line);

/**
 * \brief Make the line dirty or not.
 * \param w 1 if now dirty, 0 otherwise.   
 */
void coherence_dirty_line(struct coherence *this, struct line *line, int w);


/**
 * \brief Init the state machine.
 * \param cache_coherence Precise which coherence must be used. 
 * \param coherence Must be previously allocated.
 * \note This function must be used once only for each cache line.  
 */
void coherence_init(struct coherence *this, enum cache_coherence coher);

#endif
