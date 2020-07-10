### Modifications and Notes for the oneString controllers

#### Paint and Body

**cases, improved body materials, paint, stencils, body shape**
This can all happen on an individual basis, and allows for some wide-open customization. One only needs to do a Google image search of “Diddley Bow” or “Cigar Box guitar” to see a mass amount of potential borrowed from our devices analog cousin.

#### Interface and Layout

**sensor types, sensor positions, added buttons, knobs, or new sensors**
As the “Arduino” sees most of these sensors as an “analog input” there is potential to replace any sensor, use different shape sensors, and relocate them using ribbon cable. A simple example would be to use a long pressure sensor (I've tested Sparkfun.com SEN-09674 ) directly underneath the softPot sensor and rerouting it with a small run of 2-wire ribbon cable folded under the PCB. Though not all mode settings would be useful, this would create one pressure+position sensitive strip similar to a traditional 80's era ribbon controller.

#### Function

**special effects, new settings, new uses accompanying new sensors**


The heart of these possible changes will be in software. I expect to catalog different versions of the “Arduino” firmware on the wiki as designs progress.

Currently, there is one “reserved mode” at setting 3-2 (ON-ON-ON-OFF). Suggestions for this mode or changes to other modes should be made at the forums to gain a consensus.

If you've decided to rewrite some or all of the firmware for your own use, and would like to share it with the community. Drop me an email, and I can add it to the main download page.

#### Position Markings

**visual or tactile markings to locate notes**
As people start using the instrument, there may be a need for various methods of marking the instruments length in order to have a visual reference to octave and note location. This may be particular tricky as settings allow for different tunings and octave ranges.

I am currently marking in the body space above the sensor when held in standard position.

As these develop, I'd like to work together to try and find a way to template various markings and catalog those here for reference or download. (Format up for discussion)

Known Measurements The useful part of the standard sensor (24” instrument) is ~500mm. Measuring from the Bass end of the sensor, here are some useful mathematical notes (This needs to be confirmed on multiple sensors).

<div class="table sectionedit5"><table class="inline">
	<tr class="row0">
		<th class="col0 centeralign">  Measurement  </th><th class="col1 centeralign">   Steps: 1 Octave  </th><th class="col2 centeralign">   Steps: 2 octaves  </th><th class="col3 rightalign">   Steps: 4 octaves </th>
	</tr>
	<tr class="row1">
		<td class="col0">75mm</td><td class="col1 rightalign">   1</td><td class="col2 rightalign">   3</td><td class="col3 rightalign">   6</td>
	</tr>
	<tr class="row2">
		<td class="col0">130mm</td><td class="col1 rightalign">   2</td><td class="col2 rightalign">   ~4.5</td><td class="col3 rightalign">   9</td>
	</tr>
	<tr class="row3">
		<td class="col0">175mm</td><td class="col1 rightalign">   3</td><td class="col2 rightalign">   6</td><td class="col3 rightalign">   +12 (1 octave)</td>
	</tr>
	<tr class="row4">
		<td class="col0">242mm</td><td class="col1 rightalign">   4</td><td class="col2 rightalign">   8</td><td class="col3 rightalign">   16</td>
	</tr>
	<tr class="row5">
		<td class="col0">298mm</td><td class="col1 rightalign">   5</td><td class="col2 rightalign">   10</td><td class="col3 rightalign">   20</td>
	</tr>
	<tr class="row6">
		<td class="col0">340mm</td><td class="col1 rightalign">   6</td><td class="col2 rightalign">   +12 (1 octave)</td><td class="col3 rightalign">   +24 (2 octaves)</td>
	</tr>
	<tr class="row7">
		<td class="col0">382mm</td><td class="col1 rightalign">   7</td><td class="col2 rightalign">   14</td><td class="col3 rightalign">   28</td>
	</tr>
	<tr class="row8">
		<td class="col0">415mm</td><td class="col1 rightalign">   8</td><td class="col2 rightalign">   16</td><td class="col3 rightalign">   32</td>
	</tr>
	<tr class="row9">
		<td class="col0">442mm</td><td class="col1 rightalign">   9</td><td class="col2 rightalign">   18</td><td class="col3 rightalign">   +36(3 octaves)</td>
	</tr>
	<tr class="row10">
		<td class="col0">465mm</td><td class="col1 rightalign">   10</td><td class="col2 rightalign">   20</td><td class="col3 rightalign">   40</td>
	</tr>
	<tr class="row11">
		<td class="col0">484mm</td><td class="col1 rightalign">   11</td><td class="col2 rightalign">   22</td><td class="col3 rightalign">   44</td>
	</tr>
	<tr class="row12">
		<td class="col0 leftalign">500mm   </td><td class="col1">+12  (1 octave)</td><td class="col2 rightalign">   +24   (2 octave)</td><td class="col3 rightalign">   +48(4 octave)</td>
	</tr>
</table></div>

This table is not super useful for all octaves, but is a mathematical way of starting to understand useful common positions and their relationships. Perhaps graphic wedges pointing to 3rds & 5ths may be more useful for some musicians.

#### Temporary Markings

decal, sticker and tape markings may be an excellent choice for those without woodworking tools or testing new layout ideas.

I've had very good success using colored artists tape and a standard office hole-punch to create temporary “dots” to mark note positions.

Not yet reviewed, but looks like many of these designs could work: [Inlaysticker.com](https://www.inlaysticker.com/)

Similar on eBay: ebay search “[guitar inlay stickers](https://www.ebay.com/sch/i.html?_nkw=guitar+inlay+stickers)"

#### Permanent Markings


Like any string instrument, Inlay markers are possible and relatively simple to create with the right tools. A simple approach may be using pre-cut inlays from a luthier supplier:

I have had positive experiences with products from here for other projects: [Stewart MacDonald](https://www.stewmac.com/SiteSearch/?search=pre-cut%20inlays) : pre-cut inlays, and blanks.

Here's a great tutorial on doing your own complex inlays: [Luthier Supply Inlay Tutorial](https://www.luthiersupply.com/howto_page.html)

Luthier Supply pre-cut inlay markers [Luthier Supply](https://www.luthiersupply.com/dotsstrips.html)
