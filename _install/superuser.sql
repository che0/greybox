INSERT INTO clovek (
	clovek_ID,
	jmeno,
	prijmeni,
	prava_lidi,
	prava_kluby,
	prava_souteze,
	prava_debaty
) values (
	1,
	'John',
	'Admin',
	3,
	2,
	2,
	1
);

INSERT INTO login (
	clovek_ID,
	username,
	password
) values (
	1,
	'admin',
	md5('admin')
);