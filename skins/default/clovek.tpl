
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

<!-- BEGIN clovek_detail_kontakty_empty -->
<tr><td colspan="2">{line_empty}</td></tr>
<!-- END clovek_detail_kontakty_empty -->

<!-- BEGIN clovek_detail_kontakty_tail -->
</table>
<!-- END clovek_detail_kontakty_tail -->
