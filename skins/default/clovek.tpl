<!-- BEGIN clovek_list_debaters_head -->
<p>{list}: <a href="clovek.php?akce=vsichni">{everyone}</a> | <a href="clovek.php?akce=rozhodci">{judges}</a> | {debaters}</p>
<table class="visible">
<tr><th>{name}</td><th>{club}</th><th>{team}</th><th>{ipoints}</th></tr>
<!-- END clovek_list_debaters_head -->

<!-- BEGIN clovek_list_all_head -->
<p>{list}: {everyone} | <a href="clovek.php?akce=rozhodci">{judges}</a> | <a href="clovek.php?akce=debateri">{debaters}</a></p>
<table class="visible">
<tr><th>{name}</td><th>{club}</th><th>{team}</th><th>{ipoints}</th></tr>
<!-- END clovek_list_all_head -->
<!-- BEGIN clovek_list_all_line -->
<tr><td>{line_name}</td><td>{line_club}</td><td>{line_team}</td><td>{line_ipoints}</td></tr>
<!-- END clovek_list_all_line -->
<!-- BEGIN clovek_list_all_empty -->
<tr><td colspan="4">{line_empty}</td></tr>
<!-- END clovek_list_all_empty -->
<!-- BEGIN clovek_list_all_tail -->
<tr><td colspan="4">{num_rows}: {line_total}</td></tr>
</table>
<!-- END clovek_list_all_tail -->

<!-- BEGIN clovek_list_judges_head -->
<p>{list}: <a href="clovek.php?akce=vsichni">{everyone}</a> | {judges} | <a href="clovek.php?akce=debateri">{debaters}</a></p>
<table class="visible">
<tr><th>{name}</td><th>{club}</th><th>{languages}</th><th>{place}</td><th>{ipoints}</th></tr>
<!-- END clovek_list_judges_head -->
<!-- BEGIN clovek_list_judges_line -->
<tr><td>{line_name}</td><td>{line_club}</td><td>{line_languages}</td><td>{line_place}</td><td>{line_ipoints}</td></tr>
<!-- END clovek_list_judges_line -->
<!-- BEGIN clovek_list_judges_empty -->
<tr><td colspan="5">{line_empty}</td></tr>
<!-- END clovek_list_judges_empty -->
<!-- BEGIN clovek_list_judges_tail -->
<tr><td colspan="5">{num_rows}: {line_total}</td></tr>
</table>
<!-- END clovek_list_judges_tail -->

<!-- BEGIN clovek_detail_kontakty_head -->
<h2>{contacts}</h2>
<table class="visible">
<!-- END clovek_detail_kontakty_head -->
<!-- BEGIN clovek_detail_kontakty_line -->
<tr><td>{line_type}</td><td>{line_tx}</td></tr>
<!-- END clovek_detail_kontakty_line -->
<!-- BEGIN clovek_detail_kontakty_tail -->
</table>
<!-- END clovek_detail_kontakty_tail -->

<!-- BEGIN clovek_detail_kluby_head -->
<h2><a name="klub">{club}</a></h2>
<table class="visible">
<!-- END clovek_detail_kluby_head -->
<!-- BEGIN clovek_detail_kluby_line -->
<tr><td>{line_role}</td><td>{line_club}</td><td>({line_season})</td></tr>
<!-- END clovek_detail_kluby_line -->
<!-- BEGIN clovek_detail_kluby_tail -->
</table>
<!-- END clovek_detail_kluby_tail -->

<!-- BEGIN clovek_detail_debaty_head -->
<h2><a name="debaty">{debates}</a></h2>
<table class="visible">
<tr><th>{date}</th><th>{affirmative}</th><th>{negative}</th><th>{result}</th><th>{ipoints}</th></tr>
<!-- END clovek_detail_debaty_head -->
<!-- BEGIN clovek_detail_debaty_line -->
<tr><td>{line_date}</td><td>{line_affirmative}</td><td>{line_negative}</td><td>{line_result}</td><td>{line_ipoints}</td></tr>
<!-- END clovek_detail_debaty_line -->
<!-- BEGIN clovek_detail_debaty_tail -->
</table>
<!-- END clovek_detail_debaty_tail -->

<!-- BEGIN clovek_detail_akreditace_head -->
<h2><a name="akreditace">{accreditations}</a></h2>
<table class="visible">
<tr><th>{season}</th><th>{languages}</th><th>{place}</th><th>{ipoints}</tr>
<!-- END clovek_detail_akreditace_head -->
<!-- BEGIN clovek_detail_akreditace_line -->
<tr><td>{line_season}</td><td>{line_languages}</td><td>{line_place}</td><td>{line_ipoints}</td></tr>
<!-- END clovek_detail_akreditace_line -->
<!-- BEGIN clovek_detail_akreditace_tail -->
</table>
<!-- END clovek_detail_akreditace_tail -->

<!-- BEGIN clovek_detail_turnaje_head -->
<h2><a name="turnaje">{tournaments}</h2>
<table class="visible">
<tr><th>{date_from}</th><th>{date_to}</th><th>{title}</th><th>{ipoints}</tr>
<!-- END clovek_detail_turnaje_head -->
<!-- BEGIN clovek_detail_turnaje_line -->
<tr><td>{line_from}</td><td>{line_to}</td><td>{line_title}</td><td>{line_ipoints}</td></tr>
<!-- END clovek_detail_turnaje_line -->
<!-- BEGIN clovek_detail_turnaje_tail -->
</table>
<!-- END clovek_detail_turnaje_tail -->

<!-- BEGIN clovek_detail_ibody_head -->
<h2>{ipoints_long}</h2>
<table class="visible">
<tr><th>{season}</th><th><a href="#debaty">{debates}</a></th><th><a href="#akreditace">{judging}</a></th><th><a href="#klub">{coaching}</a></th><th><a href="#turnaje">{tournaments}</a></td><th>{total}</tr>
<!-- END clovek_detail_ibody_head -->
<!-- BEGIN clovek_detail_ibody_line -->
<tr><td>{line_season}</td><td>{line_debates}</td><td>{line_judging}</td><td>{line_coaching}</td><td>{line_tournaments}</td><td>{line_total}</td></tr>
<!-- END clovek_detail_ibody_line -->
<!-- BEGIN clovek_detail_ibody_tail -->
</table>
<!-- END clovek_detail_ibody_tail -->

<!-- BEGIN clovek_new -->
<!-- END clovek_new -->
