<?php
/*
 * greybox
 * $Id: czech.php,v 1.69 2007/03/05 15:31:59 che0 Exp $
 * czech language definition file
 */

$lang['release_notes_filename'] = 'relnotes_cz.inc';
$lang['tail_footnote'] = 'greybox, <a href="http://greybox.sourceforge.net/">greybox.sourceforge.net</a>, &copy;2004 Petr Novák<br />Distribuováno pod licencí <a href="http://www.gnu.org/copyleft/gpl.html">GNU General Public License</a>. Používá <a href="http://www.php.net/">PHP</a>, <a href="http://www.mysql.com/products/mysql/">MySQL</a> a <a href="http://www.christian-seiler.de/projekte/php/bbcode/">BB-Code parser Christiana Seilera</a>.<br />
Stránky obsahují <a href="http://validator.w3.org/check?uri=referer">XHTML</a> a <a href="http://jigsaw.w3.org/css-validator/check/referer">CSS</a>, doporučen je tedy <a href="http://www.mozilla.org/products/firefox/">webový prohlížeč</a>.';

// general
$lang['anonymous'] = 'nepřihlášený uživatel';
$lang['log in'] = 'přihlásit';
$lang['log out'] = 'odhlásit';

$lang['link_homepage'] = 'domů';
$lang['link_everyone'] = 'lidé';

$lang['submit'] = 'odeslat';
$lang['language changed'] = 'jazyk změněn';

// login/logout
$lang['logout ok'] = 'odhlášení ok';
$lang['login'] = 'přihlášení';
$lang['username'] = 'login';
$lang['password'] = 'heslo';
$lang['on login page'] = 'přihlašování';
$lang['logging out'] = 'odhlašování';
$lang['access denied'] = 'přístup odepřen';
$lang['login ok'] = 'přihlášení ok';

// clovek
$lang['list'] = 'seznam';
$lang['people'] = 'lidé';
$lang['everyone'] = 'všichni';
$lang['judges'] = 'rozhodčí';
$lang['debaters'] = 'debatéři';
$lang['name'] = 'jméno';
$lang['club'] = 'klub';
$lang['team'] = 'tým';
$lang['ipoints'] = 'IB';
$lang['place'] = 'místo';
$lang['languages'] = 'jazyk';
$lang['no records'] = 'nenalezeny žádné záznamy';
$lang['number of records'] = 'počet záznamů';
$lang['contacts'] = 'kontakty';
$lang['phone'] = 'telefon';
$lang['email'] = 'e-mail';
$lang['postal'] = 'adresa';
$lang['icq'] = 'icq';
$lang['jabber'] = 'jabber';
$lang['web'] = 'web';
$lang['member'] = 'člen';
$lang['coach'] = 'trenér';
$lang['debates'] = 'debaty';
$lang['date'] = 'datum';
$lang['affirmative'] = 'afirmace';
$lang['negative'] = 'negace';
$lang['result'] = 'výsledek';
$lang['season'] = 'sezóna';
$lang['accreditations'] = 'akreditace';
$lang['tournaments'] = 'turnaje';
$lang['date_from'] = 'od';
$lang['date_to'] = 'do';
$lang['title'] = 'název';
$lang['judging'] = 'rozhodování';
$lang['coaching'] = 'trenování';
$lang['total'] = 'celkem';
$lang['ipoints_long'] = 'individuální body';
$lang['add person'] = 'přidat člověka';
$lang['name_desc'] = 'křestní jméno';
$lang['surname'] = 'příjmení';
$lang['surname_desc'] = '';
$lang['nick'] = 'nick';
$lang['nick_desc'] = 'přezdívka pro snadnější identifikaci';
$lang['club_desc'] = 'klub, pod který patří';
$lang['born'] = 'narozen';
$lang['born_desc'] = 'yyyy-mm-dd';
$lang['active debater'] = 'aktivní debatér';
$lang['active debater_desc'] = 'aktivní debatér, člen ADK';
$lang['yes'] = 'ano';
$lang['no'] = 'ne';
$lang['comment'] = 'komentář';
$lang['comment_desc'] = 'veřejně přístupný komentář';
$lang['edit'] = 'upravit';
$lang['no club'] = '-žádný-';
$lang['add ok'] = 'přidáno v pořádku';
$lang['add failed'] = 'přidání selhalo';
$lang['edit ok'] = 'úprava v pořádku';
$lang['edit failed'] = 'úprava selhala';
$lang['delete'] = 'smazat';
$lang['add'] = 'přidat';
$lang['public'] = 'veřejný';
$lang['registered'] = 'pro přihlášené';
$lang['club only'] = 'jen pro klub';
$lang['delete_desc'] = 'úplně dočista smazat člověka';
$lang['im sure'] = 'jsem si zcela jist';
$lang['kill table ok'] = 'z tabulky \'%s\' úspěšně smazáno';
$lang['kill table failed'] = 'mazání z tabulky \'%s\' selhalo';
$lang['no confirm'] = 'nepotvrdil jste smazání';
$lang['access'] = 'přístup';
$lang['username_desc'] = '';
$lang['password_desc'] = '';
$lang['password_desc2'] = 'pro kontrolu';
$lang['passwords do not match'] = 'hesla se neshodují';
$lang['change password'] = 'změnit heslo';
$lang['password changed'] = 'heslo úspěšně změněno';
$lang['delete account'] = 'smazat účet';
$lang['delete account_desc'] = '';
$lang['delete ok'] = 'smazání proběhlo v pořádku';
$lang['delete failed'] = 'mazání selhalo';
$lang['access people'] = 'přistup (lidé)';
$lang['access people0'] = 'normální uživatel';
$lang['access people1'] = 'jen v klubu';
$lang['access people2'] = 'všichni, vč.akreditací';
$lang['access people3'] = 'úplně vše, vč. přístupů';
$lang['access people_desc'] = '';
$lang['access clubs'] = 'přístup (kluby)';
$lang['access clubs0'] = 'nic';
$lang['access clubs1'] = 'svůj klub';
$lang['access clubs2'] = 'všechny';
$lang['access clubs_desc'] = '';
$lang['access competitions'] = 'přístup (soutěže)';
$lang['access competitions0'] = 'nic';
$lang['access competitions1'] = 'turnaje';
$lang['access competitions2'] = 'turnaje a soutěže';
$lang['access competetions_desc'] = '';
$lang['access debates'] = 'přístup (debaty)';
$lang['access debates0'] = 'nic';
$lang['access debates1'] = 'může upravovat';
$lang['access debates_desc'] = '';

// index
$lang['teams'] = 'týmy';
$lang['user'] = 'uživatel';

// klub
$lang['clubs'] = 'kluby';
$lang['members'] = 'členové';
$lang['add team'] = 'přidat tým';
$lang['add club'] = 'přidat klub';
$lang['active debater_short'] = 'debatér';
$lang['active'] = 'aktivní';

$lang['club name'] = 'jméno klubu';
$lang['club name_desc'] = '';
$lang['short name'] = 'zkrácené jméno';
$lang['short name_desc'] = 'do výpisů a podobně';
$lang['club place_desc'] = '';

// teze
$lang['language'] = 'jazyk';
$lang['Language'] = 'Jazyk';
$lang['resolutions'] = 'teze';
$lang['resolution'] = 'teze';
$lang['details'] = 'více';
$lang['add resolution'] = 'přidej tezi';
$lang['save'] = 'uložit';
$lang['resolution details'] = 'detaily teze';

$lang['access competitions_desc'] = '';
$lang['official in'] = 'oficiální pro';
$lang['competition'] = 'soutěž';
$lang['edit resolution'] = 'úprava teze';
$lang['update ok'] = 'upraveno';
$lang['competitions'] = 'soutěže';
$lang['lang'] = 'jazyk';
$lang['edit competition'] = 'úprava soutěže';
$lang['add competition'] = 'přidání soutěže';
$lang['official resolution'] = 'oficiální teze';
$lang['team rankings'] = 'žebříček týmů';
$lang['add new'] = 'přidat nový';
$lang['new official resolution'] = 'přidat oficiální tezi';
$lang['locked'] = 'zamčeno';
$lang['edit official resolution'] = 'upravit oficiální tezi';
$lang['rank'] = '#';
$lang['pts'] = 'body';
$lang['tournament'] = 'turnaj';
$lang['aff'] = 'aff';
$lang['neg'] = 'neg';
$lang['debated'] = 'debatováno';
$lang['team in the competition'] = 'tým v soutěži';
$lang['total points'] = 'body celkem';
$lang['debate details'] = 'detaily';
$lang['add tournament'] = 'přidat turnaj';
$lang['edit tournament'] = 'úprava turnaje';
$lang['person'] = 'člověk';
$lang['organizer'] = 'organizátor';
$lang['new person'] = 'přidat člověka';
$lang['resolution update ok'] = 'teze upravena';
$lang['person update ok'] = 'člověk upraven';
$lang['role'] = 'role';
$lang['edit team'] = 'upravit tým';
$lang['team name'] = 'tým';
$lang['disabled'] = 'ne';
$lang['add member'] = 'přidat člena';
$lang['team update ok'] = 'tým upraven';
$lang['refusing to delete member with recorded debates'] = 'nelze smazat člena se záznamy o debatách';
$lang['status'] = 'status';
$lang['edit club'] = 'upravit klub';
$lang['person insert ok'] = 'člověk přidán';
$lang['person delete ok'] = 'člověk smazán';
$lang['edit person'] = 'upravit člověka';
$lang['club insert ok'] = 'klub přidán';
$lang['club update ok'] = 'klub upraven';
$lang['club delete ok'] = 'klub smazán';
$lang['competition insert ok'] = 'soutěž přidána';
$lang['competition update ok'] = 'soutěž upravena';
$lang['competition delete ok'] = 'soutěž smazána';
$lang['tournament insert ok'] = 'turnaj přidán';
$lang['tournament update ok'] = 'turnaj upraven';
$lang['tournament delete ok'] = 'turnaj smazán';
$lang['team insert ok'] = 'tým přidán';
$lang['team delete ok'] = 'tým smazán';
$lang['coach update ok'] = 'trenér upraven';
$lang['no record'] = 'záznam nenalezen';
$lang['link'] = 'link';
$lang['club positions'] = 'funkce';
$lang['accrediation update ok'] = 'akreditace upravena';
$lang['separate by commas: en,cz'] = 'oddělujte čárkami: en,cz';
$lang['new accreditation'] = 'přidat akreditaci';
$lang['contact update ok'] = 'kontakt upraven';
$lang['new contact'] = 'přidat kontakt';
$lang['type'] = 'druh';
$lang['contact'] = 'kontakt';
$lang['access levels'] = 'oprávnění';
$lang['area'] = 'oblast';
$lang['permissions update ok'] = 'práva změněna';
$lang['old'] = 'staré';
$lang['new'] = 'nové';
$lang['bad old password'] = 'staré heslo je špatně';
$lang['account insert ok'] = 'účet přidán';
$lang['account update ok'] = 'účet upraven';
$lang['account delete ok'] = 'účet smazán';
$lang['adjudicator'] = 'rozhodčí';
$lang['A1'] = 'A1';
$lang['A2'] = 'A2';
$lang['A3'] = 'A3';
$lang['N1'] = 'N1';
$lang['N2'] = 'N2';
$lang['N3'] = 'N3';
$lang['debater'] = 'debatér';
$lang['points overview'] = 'přehled IB';
$lang['refusing to set empty password'] = 'nelze nastavit prázdné heslo';
$lang['3:0'] = '3:0';
$lang['2:1'] = '2:1';
$lang['won'] = 'vyhráli';
$lang['debate'] = 'debata';
$lang['1st speaker'] = '1. řečník';
$lang['kidy'] = 'kidy';
$lang['2nd speaker'] = '2. řečník';
$lang['3rd speaker'] = '3. řečník';
$lang['adjudicators'] = 'rozhodčí';
$lang['decision'] = 'rozhodnutí';
$lang['homepage'] = 'domů';
$lang['add debate'] = 'přidat debatu';
$lang['edit debate'] = 'upravit debatu';
$lang['duplicate username'] = 'uživatelské jméno je již obsazeno';
$lang['N/A'] = 'N/A';
$lang['yyyy-mm-dd'] = 'yyyy-mm-dd';
$lang['back to debate'] = 'zpět na debatu';
$lang['organizers'] = 'organizátoři';
$lang['please note that the points and rankings are computed according to the debate cup system, which may not necessarily be identical to the one used at the actual competition'] = 'body a pořadí jsou počítány podle bodování debatního poháru, které se nemusí shodovat s bodováním této soutěže';
$lang['please note that the points and rankings are computed according to the debate cup system, which may not necessarily be identical to the one used at the actual tournament'] = 'body a pořadí jsou počítány podle bodování debatního poháru, které se nemusí shodovat s bodováním uplatňovaným na turnaji';
$lang['applies only if there are no adjudicators filled in'] = 'má smysl jen nejsou-li vyplněni rozhodčí';
$lang['inherited organizer'] = 'zděděný organizátor';
$lang['new adjudicator'] = 'přidat rozhodčího';
$lang['new organizer'] = 'přidat organizátora';
$lang['refusing to set adjudicator without a decision'] = 'rozhodčí musí mít nastaveno rozhodnutí';
$lang['persuasiveness not set, falling to default 3:0'] = 'není vybráno přesvědčivé/nepřesvědčivé rozhodnutí; nastavuji na přesvědčivé';
$lang['debate update ok'] = 'debata upravena';
$lang['organizer cannot have a decision set, resetting to N/A'] = 'organizátor nemůže rozhodovat, nuluji rozhodnutí';
$lang['adjudicator does not have accreditation for competition language or season'] = 'rozhodčí nemá akreditaci pro příslušný jazyk/sezónu';
$lang['speaker %s is not an active debater, dropping'] = 'řečník %s není nastaven jako aktivní debatér, zahazuji';
$lang['speaker %s is not active member of %s team, dropping'] = 'řečník %s není aktivní člen %s týmu, zahazuji';
$lang['tournament does not match competition, changing competition'] = 'turnaj nepatří do vybrané soutěže, upravuji soutěž';
$lang['debate add ok'] = 'debata přidána';
$lang['debate deleted ok'] = 'debata smazána';
$lang['debate %s: tie votes, leaving default decision: %s'] = 'debata %s: rozhodčí vyprodukovali remízu, ponechávám předchozí rozhodnutí %s';
$lang['debate %s: recount failed'] = 'dataab %s: přepočítání výsledků selhalo';
$lang['kidy overview'] = 'přehled kidů';
$lang['sort'] = 'řazení';
$lang['sum'] = 'součet';
$lang['average'] = 'průměr';
$lang['season changed'] = 'sezóna změněna';
$lang['add member from club'] = 'přidat člena z klubu';
$lang['a person has to have at least a name'] = 'člověk musí mít zadáno alespoň jméno';
$lang['a club has to have a name'] = 'klub musí mít zadáno jméno';
$lang['a competition has to have a name'] = 'soutěž musí mít zadáno jméno';
$lang['a tournament has to have a name'] = 'turnaj musí mít zadáno jméno';
$lang['a team has to have a name'] = 'tým musí mít zadáno jméno';
$lang['resolution language does not match competition language'] = 'teze není v jazyce soutěže';
$lang['selected competition is locked'] = 'zvolená soutěž je uzamčena';
$lang['administrators'] = 'správci';
$lang['administrator'] = 'správce';
$lang['competition is locked'] = 'soutěž je uzamčena';
$lang['team cannot debate against itself'] = 'tým nemůže debatovat sám proti sobě';
$lang['speaker %s is not a member of %s team, dropping'] = 'řečník %s není členem %s týmu, zahazuji';
$lang['one person cannot debate twice in one debate (%s==%s), dropping %s'] = 'jeden řečník debatovat dvakrát  ve stejné debatě (%s==%s), zahazuji %s';
$lang['samples'] = 'vzorků';
$lang['clovek.add.achtung'] = 'Prosím ujistěte se, že člověk, kterého se snažíte přidat, v systému ještě není. Seznam všech lidí je <a href="./?page=lide">zde</a>. Upozorňuji, že pro přidání člena týmu není nutné, aby tento byl v klubu, do nějž tým přísluší. Děkuji. P.N.';
$lang['wins'] = 'výhry';
$lang['ballots'] = 'balloty';
$lang['Average ballots (aff:neg): %s:%s'] = 'Průměr výsledku (balloty aff:neg): %s:%s';
$lang['factor'] = 'faktor';
$lang['full member'] = 'člen ADK';
$lang['ranks only for full members'] = 'V žebříčcích se vyskytují pouze členové ADK.';
$lang['stats'] = 'statistiky';
$lang['total number of people in the database'] = 'celkem osob v databázi';
$lang['active debaters'] = 'aktivní debatéři';
$lang['active judges'] = 'rozhodčí';
$lang['full members'] = 'členové ADK';
$lang['full members by age'] = 'členové ADK podle věku';
$lang['unset'] = 'nevyplněno';
$lang['<15'] = '&lt;15';
$lang['15-18'] = '15-18';
$lang['18-26'] = '18-26';
$lang['26+'] = '26+';
$lang['stddev'] = 'odchylka';
$lang['global rankings'] = 'globální přehled';
$lang['include panels'] = 'včetně panelů';
$lang['single judges'] = 'jen samosoudci';
$lang['filter'] = 'filtr';
$lang['yyyy-mm-dd hh:mm:ss'] = 'yyyy-mm-dd hh:mm:ss';
$lang['league'] = 'liga';
$lang['add league'] = 'přidání ligy';
$lang['edit league'] = 'upravit ligu';
$lang['a league has to have a name'] = 'liga musí mít zadáno jméno';
$lang['league insert ok'] = 'liga přidána';
$lang['league update ok'] = 'liga upravena';
$lang['league delete ok'] = 'liga smazána';
$lang['n/a'] = 'n/a';
$lang['no league'] = '-žádná-';
$lang['league pts'] = 'liga - výtěžek';
$lang['team in the league'] = 'tým v lize';
$lang['tab value'] = 'tab';
$lang['cup'] = 'pohár';
$lang['current tab'] = 'aktuální tab';
$lang['position'] = 'poz.';
$lang['opponent'] = 'soupeř';
$lang['gain'] = 'výtěžek';
$lang['new tab'] = 'nový tab';
$lang['short text'] = 'krátký text';
$lang['tournament does not match competition, unsetting tournament'] = 'turnaj nesouhlasí se soutěží, ruším přiřazení';
$lang['show all'] = 'ukaž všechny';
$lang['%s total debates, listing %s from season %s.'] = '%s debat celkem, z toho %s v sezóně %s.';
$lang['%s: %s total debates'] = '%s: celkem %s debat';
$lang['time of the debate has to be in the past'] = 'čas debaty musí být v minulosti';
$lang['ipoints update ok'] = 'ibody aktualizovány';
$lang['separate by commas: DL,DP,SD,SD2-2'] = 'oddělujte čárkami: DL,DP,SD,SD2-2';
$lang['format'] = 'formát';
$lang['add ipoints'] = 'přidat IB';
$lang['ipts'] = 'IB';
$lang['bonus'] = 'bonus';
$lang['bonus ipoints'] = 'bonusové';
$lang['debate ipoints'] = 'z debat';
$lang['draw'] = 'remíza';
$lang['lost'] = 'prohráli';
$lang['debate %s ended up in draw, that is illegal in a league'] = 'ligová debata %s skončila remízou, to by se nemělo státá';
$lang['league %s: recount failed'] = 'liga %s: přepočet selhal';
?>
