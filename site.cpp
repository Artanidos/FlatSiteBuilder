/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "site.h"
#include <QTest>

Site::Site(QString filename)
{
    QFileInfo info(filename);
    m_filename = info.fileName();
    m_path = info.path();
}



/* divi ohne sections, rows

<div class="entry-content">
<p>MeinText</p>
</div> <!-- .entry-content -->

*/


/* divi mit sections, rows, 1 Column

<div class="entry-content">
    <div class="et_pb_section  et_pb_section_0 et_section_regular">
        <div class=" et_pb_row et_pb_row_0">
                <div class="et_pb_column et_pb_column_4_4  et_pb_column_0">
                    <div class="et_pb_text et_pb_module et_pb_bg_layout_light et_pb_text_align_left  et_pb_text_0">
                        <p>MeinText</p>
                    </div> <!-- .et_pb_text -->
               </div> <!-- .et_pb_column -->
        </div> <!-- .et_pb_row -->
    </div> <!-- .et_pb_section -->
</div> <!-- .entry-content -->

*/

/* divi mit sections, rows, 2 Columns

<div class="entry-content">
    <div class="et_pb_section  et_pb_section_0 et_section_regular">
        <div class=" et_pb_row et_pb_row_0">
            <div class="et_pb_column et_pb_column_1_2  et_pb_column_0">
                <div class="et_pb_text et_pb_module et_pb_bg_layout_light et_pb_text_align_left  et_pb_text_0">
                    <p>MeinText</p>
                </div> <!-- .et_pb_text -->
            </div> <!-- .et_pb_column -->
            <div class="et_pb_column et_pb_column_1_2  et_pb_column_1">
                <div class="et_pb_text et_pb_module et_pb_bg_layout_light et_pb_text_align_left  et_pb_text_1">
                    <p>MeinText</p>
                </div> <!-- .et_pb_text -->
            </div> <!-- .et_pb_column -->
        </div> <!-- .et_pb_row -->
    </div> <!-- .et_pb_section -->
</div> <!-- .entry-content -->

*/


/* divi 1/3 2/3

<div class="entry-content">
    <div class="et_pb_section  et_pb_section_0 et_section_regular">
        <div class=" et_pb_row et_pb_row_0">
            <div class="et_pb_column et_pb_column_1_3  et_pb_column_0">
                <div class="et_pb_text et_pb_module et_pb_bg_layout_light et_pb_text_align_left  et_pb_text_0">
                    <p>MeinText</p>
                </div> <!-- .et_pb_text -->
            </div> <!-- .et_pb_column -->
            <div class="et_pb_column et_pb_column_2_3  et_pb_column_1">
                <div class="et_pb_text et_pb_module et_pb_bg_layout_light et_pb_text_align_left  et_pb_text_1">
                    <p>MeinText</p>
                </div> <!-- .et_pb_text -->
            </div> <!-- .et_pb_column -->
        </div> <!-- .et_pb_row -->
    </div> <!-- .et_pb_section -->
</div> <!-- .entry-content -->

*/
