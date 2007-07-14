/***************************************************************************
 *   Copyright (C) 2005-2006 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

#ifndef FIFE_MAPELEVATION_H
#define FIFE_MAPELEVATION_H

#include "mapstructure.h"

#include <list>
#include <vector>

namespace FIFE {

	class MapTileGrid;
	class MapGrid;
	class Object;

	/** Contains a game level.
	 * 
	 * Or more exactly: stores (a number of) other objects that contain
	 * the actual data:
	 * \li @see MapTile
	 * \li @see Object
	 *
	 * Tiles are only graphics and they are on a separate grid.
	 *
	 * Objects can be: critters, items, walls, scenery and non-visible 
	 * entities (blockers).
	 *
	 * \todo Objects are stored in a std::list; this causes a number of
	 * problems:
	 *
	 * \li \bug Correct object drawing depends on the order in the list;
	 * this is extremly irritating for xml-maps and will break for moving
	 * objects (or when new objects are created in-game).
	 *
	 * \note Search the wiki for: fallout map geometry; the hex-grid should
	 * work with a left->right, back->front drawing order.
	 *
	 * \todo One hex-grid may be occupied by more than one object; we need
	 * to handle this case, probably with some made-up logic.
	 * (for example: objects get an (artificial) size; ~smallest~ object 
	 * drawn first, larger ones above)
	 *
	 * \todo This seems like the right place to add pathfinding/line-of-sight.
	 *
	 * \li we could use a optimised storage for objects (for rendering) anyway
	 * \li it needs to be fast (= random access), going through the std::list is
	 * no good
	 * 
	 * \note Maybe a new class \em HexGridNode (which can contain any number of
	 * Objects); the elevation could simply hold an array, similiar to m_tiles.
	 *
	 * This new class could provide functions like:
	 * bool is_move_blocked(), is_shoot_blocked(), is_sight_blocked()
	 * std::list<Object*> get_objects_in_grid()
	 * std::list<Object*> get_objects_by_type(type_id)
	 *
	 * These depend on the properties of the contained objects; and something like
	 * them (the signatures are just made up :-) will be needed.
	 *
	 * \todo Export the yet unwritten functionality into the scriptengine...
	 * ... and we are a lot closer to a game.
	 *
	 * \bug ~Inherited~ Fallout map size limitation
	 * This bug isn't written in code in *this* class, but it seems to belong here
	 * nonetheless.
	 *
	 * The problem isn't storage capacity, but the formula calculating the hexgrid
	 * position from a single integer value (div/mod 200; see object.cpp, maybe
	 * not the only case).
	 * Certainly not good, but not that important for now.
	 * 
	 */
	class MapElevation {
		public:
			typedef std::list<Object*> type_objects;

			MapElevation(const s_elevation_info& structure);
			~MapElevation();

			///////////////// LEGACY INTERFACE //////////////////////////////
			void addObject(Object* obj, bool check_drawingorder = false);
			// FIXME: This is a horrible hack to add animated floor tiles
			void addObjectFront(Object* obj);

			const type_objects& getObjects() const;

			/////////////////////// NEW INTERFACE /////////////////////
			void setTileGrid(size_t numgrids, MapTileGrid* tilegrid);
			MapTileGrid* getTileGrid(size_t);
			size_t getNumTileGrids() const;
			void setNumTileGrids(size_t numgrids);

			void setObjectGrid(size_t numgrids, MapGrid* objgrid);
			MapGrid* getObjectGrid(size_t);
			size_t getNumObjectGrids() const;
			void setNumObjectGrids(size_t numgrids);

		private:
			std::vector<MapTileGrid*> m_tilegrids;
			std::vector<MapGrid*> m_objectgrids;


			int m_width;
			int m_height;

// 			MapTile* m_tiles;
			type_objects m_objects;

			void cleanup();

			MapElevation(const MapElevation&);
			MapElevation& operator=(const MapElevation&);
	};

}//FIFE

#endif
/* vim: set noexpandtab: set shiftwidth=2: set tabstop=2: */