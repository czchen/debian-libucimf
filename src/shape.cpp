/*
 * UCIMF - Unicode Console InputMethod Framework                                                    
 *
 * Copyright (c) 2006-2007 Chun-Yu Lee (Mat) and Open RazzmatazZ Laboratory (OrzLab)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "shape.h"
#include "graphport.h"
#include "type.h"
#include "font.h"

void Rect::draw( GraphPort* gp)
{
  if( be_Filled )
  {
    gp->FillRect( 0, 0, width , height, color );
  }
  else
  {
    gp->DrawRect( 0, 0, width , height, color );
  }
}

Text::Text()
{
  height = width = 0;
  color_fg = color_bg = 0;

  Font* font = Font::instance();
  font_width = font->width();
  font_height = font->height();
  data.clear();
}

void Text::append( const ustring& ustr )
{
  ustring orig_str;

  if( !data.empty() )
  {
    orig_str = data.back();
    data.pop_back();
  }
  
  ustring new_str = orig_str + ustr ;
  data.push_back( new_str );
  
  height = font_height * data.size();
  
  int new_length = 0; 

  // Calculate new str's size
  Font* font = Font::instance();
  for( size_t i=0; i< new_str.size(); i++ )
  {
    //new_length += font->length( new_str[i] );
    Font::Glyph *glyph = font->getGlyph( new_str[i] );

    if( !glyph ){                                                                                   
	    new_length += font->width();
    }  
    else{
	    int delta = glyph->width > font->width()/2 ? glyph->width : font->width()/2 ;
	    new_length += delta + glyph->left;
    }
  }
  
  if( new_length > width )
  {
    width = new_length;
  }

}

void Text::append_next( const ustring& ustr )
{
  data.push_back( ustr );
  
  height = font_height * data.size();

  int new_length = 0;
  // Calculate new str's size
  Font* font = Font::instance();
  for( size_t i=0; i< ustr.size(); i++ )
  {
    //new_length += font->length( ustr[i] );
    Font::Glyph *glyph = font->getGlyph( ustr[i] );
    if( !glyph ){                                                                                   
	    new_length += font->width();
    }  
    else{
	    int delta = glyph->width > font->width()/2 ? glyph->width : font->width()/2 ;
	    new_length += delta + glyph->left;
    }
  }

  if( new_length > width )
  {
    width = new_length;
  }

}

void Text::draw( GraphPort* gp)
{
  for( size_t i=0; i< data.size() ; i++ )
  {
    int x = 0;
    int font_h = font_height -1;
    for( int j=0; j < data[i].size() ; j++ )
    {
      x = gp->OutChar( x, i*font_h, color_fg, color_bg, data[i][j] );
    }
  }
}
