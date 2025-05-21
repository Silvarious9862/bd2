--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

-- Started on 2024-12-08 23:25:52

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;


SET default_table_access_method = heap;

--
-- TOC entry 217 (class 1259 OID 16388)
-- Name: carriers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.carriers (
    id bigint NOT NULL,
    company_name character varying(64) NOT NULL,
    inn bigint NOT NULL,
    website character varying(32),
    phone character varying(13) DEFAULT NULL::character varying,
    email character varying(64) DEFAULT NULL::character varying
);


ALTER TABLE public.carriers OWNER TO postgres;

--
-- TOC entry 218 (class 1259 OID 16393)
-- Name: documents; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.documents (
    id bigint NOT NULL,
    payment bigint NOT NULL,
    ttn bigint NOT NULL,
    contract bigint NOT NULL
);


ALTER TABLE public.documents OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 16396)
-- Name: employee; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.employee (
    id integer NOT NULL,
    badge integer NOT NULL,
    lastname character varying NOT NULL,
    firstname character varying,
    experience integer,
    carrier_id integer NOT NULL
);


ALTER TABLE public.employee OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 16401)
-- Name: employee_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.employee_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.employee_id_seq OWNER TO postgres;

--
-- TOC entry 4927 (class 0 OID 0)
-- Dependencies: 220
-- Name: employee_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.employee_id_seq OWNED BY public.employee.id;


--
-- TOC entry 221 (class 1259 OID 16402)
-- Name: employee_position; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.employee_position (
    employee_id integer NOT NULL,
    position_id integer NOT NULL
);


ALTER TABLE public.employee_position OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 16405)
-- Name: logs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.logs (
    id bigint NOT NULL,
    shipped_at timestamp without time zone NOT NULL,
    delivered_at timestamp without time zone DEFAULT (now())::timestamp without time zone NOT NULL,
    price integer,
    docs_set bigint NOT NULL,
    transport_id bigint NOT NULL,
    carrier_id bigint NOT NULL,
    employee_id bigint NOT NULL,
    CONSTRAINT logs_check CHECK ((delivered_at > shipped_at))
);


ALTER TABLE public.logs OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16410)
-- Name: position; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."position" (
    id integer NOT NULL,
    name character varying NOT NULL,
    salary integer,
    start_of_work time without time zone,
    end_of_work time without time zone
);


ALTER TABLE public."position" OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 16415)
-- Name: position_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.position_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.position_id_seq OWNER TO postgres;

--
-- TOC entry 4928 (class 0 OID 0)
-- Dependencies: 224
-- Name: position_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.position_id_seq OWNED BY public."position".id;


--
-- TOC entry 225 (class 1259 OID 16416)
-- Name: transport; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.transport (
    id bigint NOT NULL,
    car_model character varying(32) NOT NULL,
    insurance date,
    maintenance date,
    vin character varying(32) NOT NULL,
    company_id bigint
);


ALTER TABLE public.transport OWNER TO postgres;

--
-- TOC entry 4722 (class 2604 OID 16419)
-- Name: employee id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee ALTER COLUMN id SET DEFAULT nextval('public.employee_id_seq'::regclass);


--
-- TOC entry 4724 (class 2604 OID 16420)
-- Name: position id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."position" ALTER COLUMN id SET DEFAULT nextval('public.position_id_seq'::regclass);


--
-- TOC entry 4913 (class 0 OID 16388)
-- Dependencies: 217
-- Data for Name: carriers; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.carriers VALUES (1, 'Orn Inc', 68910809, 'http://serene-lymphocyte.biz', '609-00-80', 'Cordell_Glover@gmail.com');
INSERT INTO public.carriers VALUES (2, 'MacGyver - Keeling', 35078558, 'https://rubbery-help.name', '397-28-92', 'Ludie_Larkin25@gmail.com');
INSERT INTO public.carriers VALUES (3, 'King Inc', 76584617, 'https://cool-logistics.org', '886-02-39', 'Devin_Carroll@gmail.com');
INSERT INTO public.carriers VALUES (4, 'Gleichner LLC', 56919182, 'http://classic-funding.biz', '861-03-96', 'Keara.Feeney@yahoo.com');
INSERT INTO public.carriers VALUES (5, 'Graham - Kertzmann', 48913945, 'https://gracious-justice.info', '422-11-36', 'Morgan.Schowalter7@hotmail.com');
INSERT INTO public.carriers VALUES (6, 'Koss Inc', 18900733, 'https://present-comma.com', '578-29-05', 'Arturo.Kunde@yahoo.com');
INSERT INTO public.carriers VALUES (7, 'Wyman - Waelchi', 87734970, 'http://healthy-capitulation.org', '225-49-99', 'Domenico_Jakubowski71@hotmail.com');
INSERT INTO public.carriers VALUES (8, 'Cremin LLC', 86621960, 'http://beautiful-coinsurance.net', '323-29-28', 'Parker_Huel@gmail.com');
INSERT INTO public.carriers VALUES (9, 'Kessler - Schamberger', 30188484, 'https://self-assured-spring.name', '629-19-22', 'Gabe9@hotmail.com');
INSERT INTO public.carriers VALUES (10, 'Hammes Group', 26550868, 'https://amused-leopard.com', '278-57-19', 'Dorthy.Cartwright70@hotmail.com');
INSERT INTO public.carriers VALUES (11, 'Schaden Group', 21326120, 'https://confused-celsius.name', '886-45-31', 'Kyla46@hotmail.com');
INSERT INTO public.carriers VALUES (12, 'Corkery, Veum and Jerde', 95582284, 'https://impartial-avenue.name', '561-19-35', 'Don.Price@yahoo.com');
INSERT INTO public.carriers VALUES (13, 'Hintz - Larkin', 63704789, 'https://whispered-genocide.org', '348-39-42', 'Ubaldo_Hodkiewicz9@yahoo.com');
INSERT INTO public.carriers VALUES (14, 'Davis - Kuhn', 85255184, 'https://superior-fee.biz', '469-84-60', 'Jaeden45@gmail.com');
INSERT INTO public.carriers VALUES (15, 'Gorczany Group', 53680440, 'http://vibrant-rainstorm.com', '829-49-95', 'Sigurd_Purdy18@yahoo.com');
INSERT INTO public.carriers VALUES (16, 'Kassulke - Fahey', 12370537, 'https://forceful-violation.name', '164-56-38', 'Lavina47@hotmail.com');
INSERT INTO public.carriers VALUES (17, 'Lueilwitz, Mayer and Larson', 52637656, 'http://rusty-reunion.biz', '190-03-37', 'Idell.Welch55@hotmail.com');
INSERT INTO public.carriers VALUES (18, 'Bosco and Sons', 33704832, 'https://well-documented-chem.net', '534-37-30', 'Julio_Moore@hotmail.com');
INSERT INTO public.carriers VALUES (19, 'Bartoletti - Stamm', 78186363, 'https://kindhearted-senator.biz', '092-64-01', 'Aniyah.Wunsch@gmail.com');
INSERT INTO public.carriers VALUES (20, 'Gleason Group', 44437188, 'http://amazing-change.biz', '563-26-53', 'Nils.Farrell@gmail.com');
INSERT INTO public.carriers VALUES (21, 'Swift, Shanahan and Walker', 85777366, 'https://intentional-yarmulke.com', '496-24-80', 'Brook18@hotmail.com');
INSERT INTO public.carriers VALUES (22, 'Pouros Group', 15856706, 'https://rosy-aquarium.name', '268-87-53', 'Jamir_Langworth@hotmail.com');
INSERT INTO public.carriers VALUES (23, 'Kassulke, OHara and Renner', 89244285, 'http://trustworthy-immigrant.biz', '836-45-21', 'Jeromy88@gmail.com');
INSERT INTO public.carriers VALUES (24, 'Grimes, Braun and Gerlach', 85779858, 'http://unhappy-election.info', '908-91-30', 'Elwyn.Wiegand5@yahoo.com');
INSERT INTO public.carriers VALUES (25, 'Boyer Inc', 97217112, 'http://worn-intentionality.name', '169-10-02', 'Bryon_Green@gmail.com');
INSERT INTO public.carriers VALUES (26, 'Turcotte Inc', 79347011, 'https://red-parchment.com', '629-48-73', 'Brook_Bode92@yahoo.com');
INSERT INTO public.carriers VALUES (27, 'Dickens Group', 32378860, 'http://recent-tissue.info', '358-12-56', 'Rey_Dickinson@gmail.com');
INSERT INTO public.carriers VALUES (28, 'Monahan and Sons', 16837047, 'https://inborn-beard.biz', '292-78-93', 'Cletus97@yahoo.com');
INSERT INTO public.carriers VALUES (29, 'Hintz, Koch and Feil', 85182201, 'https://minty-clasp.info', '724-72-30', 'Charlie.Schaefer64@gmail.com');


--
-- TOC entry 4914 (class 0 OID 16393)
-- Dependencies: 218
-- Data for Name: documents; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.documents VALUES (1, 5628642, 47678129, 844342);
INSERT INTO public.documents VALUES (2, 8890306, 33096118, 743037);
INSERT INTO public.documents VALUES (3, 5957839, 85045490, 365133);
INSERT INTO public.documents VALUES (4, 2307239, 62193793, 405689);
INSERT INTO public.documents VALUES (5, 5882305, 65812238, 849696);
INSERT INTO public.documents VALUES (6, 5329294, 76037028, 570903);
INSERT INTO public.documents VALUES (7, 3859676, 11516859, 340810);
INSERT INTO public.documents VALUES (8, 3067270, 46144782, 667879);
INSERT INTO public.documents VALUES (9, 3194151, 41227323, 831913);
INSERT INTO public.documents VALUES (10, 1647656, 73605462, 886008);
INSERT INTO public.documents VALUES (11, 1398026, 10556801, 312028);
INSERT INTO public.documents VALUES (12, 8279063, 34047097, 714088);
INSERT INTO public.documents VALUES (13, 3684212, 30899900, 662681);
INSERT INTO public.documents VALUES (14, 8994642, 84624806, 580923);
INSERT INTO public.documents VALUES (15, 1486122, 95971431, 990101);
INSERT INTO public.documents VALUES (16, 6842313, 89878002, 870781);
INSERT INTO public.documents VALUES (17, 6604920, 84656586, 495366);
INSERT INTO public.documents VALUES (18, 1711415, 47897608, 206062);
INSERT INTO public.documents VALUES (19, 1281926, 92735567, 667068);
INSERT INTO public.documents VALUES (20, 4628135, 30625308, 990048);
INSERT INTO public.documents VALUES (21, 7193789, 30213330, 657016);
INSERT INTO public.documents VALUES (22, 1432319, 75001628, 437816);
INSERT INTO public.documents VALUES (23, 7052930, 41131646, 218363);
INSERT INTO public.documents VALUES (24, 3086300, 30049322, 877732);
INSERT INTO public.documents VALUES (25, 3187509, 13272653, 393879);
INSERT INTO public.documents VALUES (26, 2542304, 24284127, 944179);
INSERT INTO public.documents VALUES (27, 6825557, 77518412, 609474);
INSERT INTO public.documents VALUES (28, 2868893, 87858219, 210052);
INSERT INTO public.documents VALUES (29, 1258975, 63596176, 293346);
INSERT INTO public.documents VALUES (30, 6233685, 33645046, 393088);
INSERT INTO public.documents VALUES (31, 3391267, 80747855, 185955);
INSERT INTO public.documents VALUES (32, 8239685, 11320601, 238739);
INSERT INTO public.documents VALUES (33, 5169783, 69777055, 405557);
INSERT INTO public.documents VALUES (34, 2019227, 49083919, 844108);
INSERT INTO public.documents VALUES (35, 3679897, 40392506, 733043);
INSERT INTO public.documents VALUES (36, 6288834, 65012407, 749079);
INSERT INTO public.documents VALUES (37, 8858951, 39744084, 883336);
INSERT INTO public.documents VALUES (38, 5118286, 61058875, 104417);
INSERT INTO public.documents VALUES (39, 3446802, 66661168, 140939);
INSERT INTO public.documents VALUES (40, 2120142, 55806965, 606498);
INSERT INTO public.documents VALUES (41, 5687618, 20070377, 761712);
INSERT INTO public.documents VALUES (42, 6330549, 73151869, 142397);
INSERT INTO public.documents VALUES (43, 3938669, 94384053, 735644);
INSERT INTO public.documents VALUES (44, 3933491, 70718923, 180995);
INSERT INTO public.documents VALUES (45, 4908940, 45468242, 436205);
INSERT INTO public.documents VALUES (46, 6150649, 24816317, 719513);
INSERT INTO public.documents VALUES (47, 8669136, 12385876, 962610);
INSERT INTO public.documents VALUES (48, 9291913, 43196600, 575538);
INSERT INTO public.documents VALUES (49, 4476963, 22915814, 922686);
INSERT INTO public.documents VALUES (50, 8309030, 92581229, 680961);
INSERT INTO public.documents VALUES (51, 3917547, 87344874, 493603);
INSERT INTO public.documents VALUES (52, 3354509, 56404178, 338388);
INSERT INTO public.documents VALUES (53, 4989839, 96144119, 265067);
INSERT INTO public.documents VALUES (54, 5240731, 35158771, 677670);
INSERT INTO public.documents VALUES (55, 3077565, 46428604, 625485);
INSERT INTO public.documents VALUES (56, 3690132, 31645299, 660354);
INSERT INTO public.documents VALUES (57, 8167528, 19056715, 905290);
INSERT INTO public.documents VALUES (58, 4490470, 23405393, 845869);
INSERT INTO public.documents VALUES (59, 2019665, 26116893, 840705);
INSERT INTO public.documents VALUES (60, 9627905, 24219731, 730927);
INSERT INTO public.documents VALUES (61, 7119159, 78608217, 903107);
INSERT INTO public.documents VALUES (62, 1385411, 67945900, 486656);
INSERT INTO public.documents VALUES (63, 8426365, 18258940, 961415);
INSERT INTO public.documents VALUES (64, 7468462, 28921458, 224198);
INSERT INTO public.documents VALUES (65, 9970957, 18307195, 493543);
INSERT INTO public.documents VALUES (66, 6827077, 51310776, 549856);
INSERT INTO public.documents VALUES (67, 7457551, 48933273, 869332);
INSERT INTO public.documents VALUES (68, 6461506, 84630889, 357608);
INSERT INTO public.documents VALUES (69, 2557978, 38716719, 232301);
INSERT INTO public.documents VALUES (70, 9067182, 96397427, 381002);
INSERT INTO public.documents VALUES (71, 8879833, 31359052, 794088);
INSERT INTO public.documents VALUES (72, 7335720, 91632003, 763673);
INSERT INTO public.documents VALUES (73, 1432192, 47909349, 552179);
INSERT INTO public.documents VALUES (74, 6676277, 70969969, 643661);
INSERT INTO public.documents VALUES (75, 2690010, 15794754, 488153);
INSERT INTO public.documents VALUES (76, 5403457, 96745538, 662053);
INSERT INTO public.documents VALUES (77, 5337702, 94229483, 103107);
INSERT INTO public.documents VALUES (78, 9428894, 39276954, 736694);
INSERT INTO public.documents VALUES (79, 7206631, 70690679, 432603);
INSERT INTO public.documents VALUES (80, 4906911, 11547215, 657467);
INSERT INTO public.documents VALUES (81, 7135703, 58850625, 987679);
INSERT INTO public.documents VALUES (82, 1118166, 52122673, 459781);
INSERT INTO public.documents VALUES (83, 1778674, 83626767, 659377);
INSERT INTO public.documents VALUES (84, 4107619, 55078151, 628357);
INSERT INTO public.documents VALUES (85, 8499421, 60710835, 907034);
INSERT INTO public.documents VALUES (86, 2990649, 55418174, 940905);
INSERT INTO public.documents VALUES (87, 6987219, 47936421, 254401);
INSERT INTO public.documents VALUES (88, 3154472, 20634571, 216789);
INSERT INTO public.documents VALUES (89, 5531931, 91520953, 633735);
INSERT INTO public.documents VALUES (90, 2985916, 87725399, 954937);
INSERT INTO public.documents VALUES (91, 1222737, 31704282, 476116);
INSERT INTO public.documents VALUES (92, 5543486, 68569337, 651140);
INSERT INTO public.documents VALUES (93, 7237830, 20994129, 658838);
INSERT INTO public.documents VALUES (94, 1829463, 88319411, 117560);
INSERT INTO public.documents VALUES (95, 1810535, 93491724, 836026);
INSERT INTO public.documents VALUES (96, 3174127, 56240317, 362827);
INSERT INTO public.documents VALUES (97, 3158064, 11070390, 840570);
INSERT INTO public.documents VALUES (98, 8096275, 16425219, 895205);
INSERT INTO public.documents VALUES (99, 4746793, 96992096, 941141);


--
-- TOC entry 4915 (class 0 OID 16396)
-- Dependencies: 219
-- Data for Name: employee; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.employee VALUES (1, 65331, 'Sauer', 'Elise', 4, 25);
INSERT INTO public.employee VALUES (2, 17019, 'Stark', 'Sadye', 18, 11);
INSERT INTO public.employee VALUES (3, 35485, 'Harvey', 'Delpha', 5, 16);
INSERT INTO public.employee VALUES (4, 42959, 'Romaguera', 'Jeffrey', 17, 23);
INSERT INTO public.employee VALUES (5, 69148, 'Witting', 'Claire', 19, 19);
INSERT INTO public.employee VALUES (6, 28567, 'Gerhold', 'Fidel', 13, 10);
INSERT INTO public.employee VALUES (7, 53817, 'Beer', 'Lavada', 4, 25);
INSERT INTO public.employee VALUES (8, 94828, 'Dooley', 'Claud', 18, 22);
INSERT INTO public.employee VALUES (9, 23868, 'Crooks', 'Madyson', 2, 19);
INSERT INTO public.employee VALUES (10, 49023, 'Langworth', 'Lottie', 4, 11);
INSERT INTO public.employee VALUES (11, 73398, 'Lueilwitz', 'Judy', 2, 10);
INSERT INTO public.employee VALUES (12, 23222, 'Dietrich', 'Zella', 18, 29);
INSERT INTO public.employee VALUES (13, 67140, 'Hayes', 'Zane', 13, 18);
INSERT INTO public.employee VALUES (14, 84648, 'Gislason', 'Destin', 9, 7);
INSERT INTO public.employee VALUES (15, 38821, 'Simonis', 'Phyllis', 18, 18);
INSERT INTO public.employee VALUES (16, 89805, 'Metz', 'Craig', 20, 3);
INSERT INTO public.employee VALUES (17, 15520, 'Bins', 'Nils', 10, 21);
INSERT INTO public.employee VALUES (18, 40591, 'Douglas', 'Grover', 11, 11);
INSERT INTO public.employee VALUES (19, 74574, 'Jerde', 'Verna', 19, 11);
INSERT INTO public.employee VALUES (20, 15350, 'Tromp', 'Trycia', 18, 22);
INSERT INTO public.employee VALUES (21, 98021, 'Pfannerstill', 'Irving', 18, 18);
INSERT INTO public.employee VALUES (22, 54635, 'Senger', 'Phyllis', 15, 5);
INSERT INTO public.employee VALUES (23, 76431, 'Schaefer', 'Aliyah', 7, 20);
INSERT INTO public.employee VALUES (24, 61871, 'Farrell', 'Raphael', 9, 25);
INSERT INTO public.employee VALUES (25, 14120, 'Lueilwitz', 'Carmen', 2, 19);
INSERT INTO public.employee VALUES (26, 10093, 'Hand', 'Reva', 12, 8);
INSERT INTO public.employee VALUES (27, 37470, 'Mueller', 'Anya', 1, 7);
INSERT INTO public.employee VALUES (28, 51050, 'Emard', 'Caitlyn', 1, 3);
INSERT INTO public.employee VALUES (29, 45090, 'Rutherford', 'Josianne', 15, 28);
INSERT INTO public.employee VALUES (30, 21184, 'Smith', 'Misael', 7, 28);
INSERT INTO public.employee VALUES (31, 59944, 'Feil', 'Ramon', 4, 28);
INSERT INTO public.employee VALUES (32, 80391, 'Wyman', 'Rigoberto', 11, 19);
INSERT INTO public.employee VALUES (33, 55094, 'Thiel', 'Rosa', 17, 28);
INSERT INTO public.employee VALUES (34, 23107, 'King', 'Cassie', 17, 21);
INSERT INTO public.employee VALUES (35, 73538, 'Nolan', 'Lolita', 10, 11);
INSERT INTO public.employee VALUES (36, 27609, 'Greenfelder', 'Hilario', 19, 12);
INSERT INTO public.employee VALUES (37, 77335, 'Wehner', 'Loren', 13, 28);
INSERT INTO public.employee VALUES (38, 66485, 'Russel', 'Eli', 19, 20);
INSERT INTO public.employee VALUES (39, 41702, 'Leannon', 'Sincere', 11, 22);
INSERT INTO public.employee VALUES (40, 21830, 'Quigley', 'Ryann', 18, 29);
INSERT INTO public.employee VALUES (41, 15830, 'Block', 'Mossie', 6, 13);
INSERT INTO public.employee VALUES (42, 91474, 'Pollich', 'Elza', 10, 7);
INSERT INTO public.employee VALUES (43, 64119, 'Frami', 'Doris', 16, 21);
INSERT INTO public.employee VALUES (44, 86867, 'Hilll', 'Ari', 3, 20);
INSERT INTO public.employee VALUES (45, 83556, 'Rice', 'Jaunita', 8, 23);
INSERT INTO public.employee VALUES (46, 76606, 'Heidenreich', 'Corene', 4, 24);
INSERT INTO public.employee VALUES (47, 17213, 'Greenfelder', 'Angela', 11, 14);
INSERT INTO public.employee VALUES (48, 14447, 'Murray', 'Linda', 11, 26);
INSERT INTO public.employee VALUES (49, 60656, 'Pagac', 'Myrl', 2, 21);
INSERT INTO public.employee VALUES (50, 23252, 'Dooley', 'Elisabeth', 13, 20);
INSERT INTO public.employee VALUES (51, 33939, 'Huels', 'Susanna', 2, 16);
INSERT INTO public.employee VALUES (52, 37927, 'Parisian', 'Tristin', 6, 26);
INSERT INTO public.employee VALUES (53, 44813, 'Borer', 'Rowland', 9, 29);
INSERT INTO public.employee VALUES (54, 48509, 'Dickens', 'Genesis', 19, 29);
INSERT INTO public.employee VALUES (55, 26210, 'Ferry', 'Jamison', 6, 9);
INSERT INTO public.employee VALUES (56, 21055, 'Maggio', 'Mavis', 1, 11);
INSERT INTO public.employee VALUES (57, 49968, 'Hermann', 'Malachi', 4, 29);
INSERT INTO public.employee VALUES (58, 58974, 'Kreiger', 'Elinor', 6, 8);
INSERT INTO public.employee VALUES (59, 43610, 'Mante', 'Beverly', 9, 14);
INSERT INTO public.employee VALUES (60, 21299, 'McDermott', 'Adriana', 12, 23);
INSERT INTO public.employee VALUES (61, 25787, 'Keebler', 'Stephanie', 20, 20);
INSERT INTO public.employee VALUES (62, 87743, 'Boehm', 'Hollie', 12, 6);
INSERT INTO public.employee VALUES (63, 43607, 'Windler', 'Loren', 19, 19);
INSERT INTO public.employee VALUES (64, 84160, 'Raynor', 'Litzy', 9, 24);
INSERT INTO public.employee VALUES (65, 66793, 'Schneider', 'Marie', 15, 24);
INSERT INTO public.employee VALUES (66, 39291, 'Gorczany', 'Lexi', 19, 4);
INSERT INTO public.employee VALUES (67, 24468, 'Keeling', 'Walter', 6, 17);
INSERT INTO public.employee VALUES (68, 45327, 'Carroll', 'Coty', 12, 24);
INSERT INTO public.employee VALUES (69, 10599, 'Nitzsche', 'Gloria', 5, 29);
INSERT INTO public.employee VALUES (70, 90713, 'Reichert', 'Ramon', 8, 11);
INSERT INTO public.employee VALUES (71, 54068, 'Baumbach', 'Jedidiah', 19, 18);
INSERT INTO public.employee VALUES (72, 11180, 'Keeling', 'Hillary', 1, 3);
INSERT INTO public.employee VALUES (73, 37204, 'Schultz', 'Antonio', 20, 24);
INSERT INTO public.employee VALUES (74, 59310, 'Kuhic', 'Oren', 5, 23);
INSERT INTO public.employee VALUES (75, 97599, 'Sporer', 'Maryjane', 7, 25);
INSERT INTO public.employee VALUES (76, 30899, 'Sporer', 'Colby', 18, 20);
INSERT INTO public.employee VALUES (77, 34716, 'Kuhlman', 'Consuelo', 12, 7);
INSERT INTO public.employee VALUES (78, 97446, 'Predovic', 'Bonnie', 7, 5);
INSERT INTO public.employee VALUES (79, 30206, 'Marvin', 'Brycen', 10, 12);
INSERT INTO public.employee VALUES (80, 95437, 'Schamberger', 'Aiden', 3, 28);
INSERT INTO public.employee VALUES (81, 45320, 'Mayert', 'Curt', 12, 28);
INSERT INTO public.employee VALUES (82, 13819, 'Schoen', 'Amaya', 17, 24);
INSERT INTO public.employee VALUES (83, 32170, 'Fay', 'Austin', 4, 11);
INSERT INTO public.employee VALUES (84, 10285, 'Blanda', 'Alysa', 13, 24);
INSERT INTO public.employee VALUES (85, 26239, 'Dickinson', 'Wilburn', 4, 14);
INSERT INTO public.employee VALUES (86, 57684, 'Bernier', 'Kiana', 16, 13);
INSERT INTO public.employee VALUES (87, 86601, 'Goyette', 'Thad', 18, 1);
INSERT INTO public.employee VALUES (88, 29219, 'Treutel', 'Nikolas', 16, 15);
INSERT INTO public.employee VALUES (89, 12983, 'Pfeffer', 'Mia', 10, 12);
INSERT INTO public.employee VALUES (90, 92627, 'Corkery', 'Edyth', 11, 6);
INSERT INTO public.employee VALUES (91, 41545, 'Lueilwitz', 'Melyssa', 11, 3);
INSERT INTO public.employee VALUES (92, 76143, 'Kshlerin', 'Alva', 7, 26);
INSERT INTO public.employee VALUES (93, 29097, 'Cormier', 'Russel', 8, 1);
INSERT INTO public.employee VALUES (94, 75908, 'Marvin', 'Mitchell', 3, 3);
INSERT INTO public.employee VALUES (95, 86173, 'Lakin', 'Alverta', 15, 5);
INSERT INTO public.employee VALUES (96, 60682, 'Mertz', 'Colt', 9, 6);
INSERT INTO public.employee VALUES (97, 54039, 'Gutkowski', 'Arlene', 4, 1);
INSERT INTO public.employee VALUES (98, 65051, 'Effertz', 'Emmet', 16, 27);
INSERT INTO public.employee VALUES (99, 36896, 'Daniel', 'Lura', 1, 25);
INSERT INTO public.employee VALUES (100, 58935, 'King', 'Gust', 14, 2);
INSERT INTO public.employee VALUES (101, 16144, 'Botsford', 'Timmothy', 12, 15);
INSERT INTO public.employee VALUES (102, 82345, 'Jenkins', 'Karelle', 2, 4);
INSERT INTO public.employee VALUES (103, 89840, 'Weissnat', 'Maverick', 1, 2);
INSERT INTO public.employee VALUES (104, 14521, 'Kuvalis', 'June', 13, 13);
INSERT INTO public.employee VALUES (105, 31291, 'Walsh', 'Trent', 6, 11);
INSERT INTO public.employee VALUES (106, 41221, 'Boyle', 'Janae', 17, 3);
INSERT INTO public.employee VALUES (107, 91904, 'Fritsch', 'Alfredo', 3, 2);
INSERT INTO public.employee VALUES (108, 29678, 'Buckridge', 'Carmine', 15, 26);
INSERT INTO public.employee VALUES (109, 54307, 'Lebsack', 'Delia', 1, 24);
INSERT INTO public.employee VALUES (110, 44893, 'Labadie', 'Rodrigo', 13, 6);
INSERT INTO public.employee VALUES (111, 84360, 'Blick', 'Melody', 16, 12);
INSERT INTO public.employee VALUES (112, 89816, 'Bins', 'Andrew', 2, 2);
INSERT INTO public.employee VALUES (113, 31115, 'Rutherford', 'Gaston', 15, 24);
INSERT INTO public.employee VALUES (114, 90304, 'Franecki', 'Tyshawn', 8, 2);
INSERT INTO public.employee VALUES (115, 46334, 'Lebsack', 'Dan', 9, 26);
INSERT INTO public.employee VALUES (116, 14401, 'Jerde', 'Dante', 18, 25);
INSERT INTO public.employee VALUES (117, 85521, 'Jakubowski', 'Leon', 15, 16);
INSERT INTO public.employee VALUES (118, 61685, 'Stracke', 'Duane', 5, 16);
INSERT INTO public.employee VALUES (119, 14595, 'Luettgen', 'Clarissa', 3, 14);
INSERT INTO public.employee VALUES (120, 22396, 'Shields', 'Jessy', 20, 16);
INSERT INTO public.employee VALUES (121, 47663, 'Spinka', 'Noemy', 20, 6);
INSERT INTO public.employee VALUES (122, 95487, 'Reynolds', 'Dan', 8, 8);
INSERT INTO public.employee VALUES (123, 44564, 'Lang', 'Russ', 6, 11);
INSERT INTO public.employee VALUES (124, 13751, 'Runolfsson', 'Eloise', 4, 27);
INSERT INTO public.employee VALUES (125, 46789, 'Brekke', 'Jeff', 11, 9);
INSERT INTO public.employee VALUES (126, 63048, 'Mayer', 'Tanya', 11, 4);
INSERT INTO public.employee VALUES (127, 43259, 'Kuvalis', 'Dashawn', 1, 2);
INSERT INTO public.employee VALUES (128, 57044, 'Bode', 'Ron', 13, 17);
INSERT INTO public.employee VALUES (129, 43675, 'Wehner', 'Elbert', 6, 6);
INSERT INTO public.employee VALUES (130, 19663, 'Crist', 'Filiberto', 1, 18);
INSERT INTO public.employee VALUES (131, 47274, 'Johnson', 'Mitchel', 15, 16);
INSERT INTO public.employee VALUES (132, 93852, 'Rempel', 'Lindsay', 10, 8);
INSERT INTO public.employee VALUES (133, 74508, 'Ferry', 'Dorian', 4, 14);
INSERT INTO public.employee VALUES (134, 68607, 'Pfannerstill', 'Jensen', 3, 18);
INSERT INTO public.employee VALUES (135, 72000, 'Schmeler', 'Roger', 3, 8);
INSERT INTO public.employee VALUES (136, 76071, 'Kutch', 'Glennie', 8, 10);
INSERT INTO public.employee VALUES (137, 95474, 'Davis', 'Colleen', 12, 23);
INSERT INTO public.employee VALUES (138, 34372, 'Funk', 'Laverne', 6, 15);
INSERT INTO public.employee VALUES (139, 91066, 'Bartell', 'Georgiana', 10, 13);
INSERT INTO public.employee VALUES (140, 18970, 'Reichert', 'Elna', 4, 5);
INSERT INTO public.employee VALUES (141, 48362, 'Ondricka', 'Ari', 1, 18);
INSERT INTO public.employee VALUES (142, 71659, 'Larson', 'Leopoldo', 3, 11);
INSERT INTO public.employee VALUES (143, 64726, 'Lindgren', 'Tristin', 8, 27);
INSERT INTO public.employee VALUES (144, 52437, 'Bashirian', 'Jermey', 1, 5);
INSERT INTO public.employee VALUES (145, 43619, 'Wehner', 'Daryl', 7, 8);
INSERT INTO public.employee VALUES (146, 19290, 'Runolfsdottir', 'Velma', 1, 14);
INSERT INTO public.employee VALUES (147, 17862, 'Spinka', 'Antonette', 11, 11);
INSERT INTO public.employee VALUES (148, 88654, 'Kris', 'Sabina', 17, 21);
INSERT INTO public.employee VALUES (149, 87760, 'Abbott', 'Amya', 19, 20);
INSERT INTO public.employee VALUES (150, 36464, 'Hayes', 'Dejah', 4, 10);
INSERT INTO public.employee VALUES (151, 65914, 'Macejkovic', 'Estelle', 4, 9);
INSERT INTO public.employee VALUES (152, 56246, 'Smith', 'Robyn', 15, 6);
INSERT INTO public.employee VALUES (153, 39883, 'Purdy', 'Gregorio', 6, 25);
INSERT INTO public.employee VALUES (154, 66303, 'O Conner', 'Patrick', 9, 1);
INSERT INTO public.employee VALUES (155, 60724, 'Ankunding', 'Lloyd', 6, 19);
INSERT INTO public.employee VALUES (156, 55423, 'Murray', 'Anika', 12, 14);
INSERT INTO public.employee VALUES (157, 59531, 'Roberts', 'Colton', 2, 4);
INSERT INTO public.employee VALUES (158, 67982, 'Aufderhar', 'Alden', 12, 4);
INSERT INTO public.employee VALUES (159, 31651, 'Rodriguez', 'Cheyenne', 11, 4);
INSERT INTO public.employee VALUES (160, 36217, 'Zboncak', 'Brown', 18, 18);
INSERT INTO public.employee VALUES (161, 86048, 'Runolfsdottir', 'Misael', 2, 12);
INSERT INTO public.employee VALUES (162, 58931, 'Keebler', 'Clarissa', 14, 27);
INSERT INTO public.employee VALUES (163, 94889, 'Little', 'Vanessa', 6, 16);
INSERT INTO public.employee VALUES (164, 22205, 'Donnelly', 'Vena', 18, 11);
INSERT INTO public.employee VALUES (165, 10338, 'Rice', 'Winston', 13, 21);
INSERT INTO public.employee VALUES (166, 64309, 'Hirthe', 'Reva', 16, 6);
INSERT INTO public.employee VALUES (167, 21696, 'Heller', 'Shany', 18, 16);
INSERT INTO public.employee VALUES (168, 16869, 'Strosin', 'Danial', 5, 17);
INSERT INTO public.employee VALUES (169, 72692, 'Kunde', 'Aliyah', 14, 23);
INSERT INTO public.employee VALUES (170, 48103, 'Cruickshank', 'Cheyenne', 11, 1);
INSERT INTO public.employee VALUES (171, 83718, 'Farrell', 'Mariano', 4, 1);
INSERT INTO public.employee VALUES (172, 45974, 'Labadie', 'Kirk', 15, 1);
INSERT INTO public.employee VALUES (173, 34481, 'Lockman', 'Lazaro', 19, 16);
INSERT INTO public.employee VALUES (174, 93788, 'Zemlak', 'Roselyn', 17, 22);
INSERT INTO public.employee VALUES (175, 59086, 'Huel', 'Peter', 6, 12);
INSERT INTO public.employee VALUES (176, 95788, 'Beatty', 'Eda', 14, 22);
INSERT INTO public.employee VALUES (177, 77200, 'White', 'Anabel', 10, 7);
INSERT INTO public.employee VALUES (178, 15591, 'Olson', 'Bill', 2, 16);
INSERT INTO public.employee VALUES (179, 52986, 'Aufderhar', 'Prince', 18, 25);
INSERT INTO public.employee VALUES (180, 96816, 'Gorczany', 'Glennie', 6, 24);
INSERT INTO public.employee VALUES (181, 64842, 'Barrows', 'Frederik', 1, 1);
INSERT INTO public.employee VALUES (182, 17571, 'Bernhard', 'Tobin', 5, 17);
INSERT INTO public.employee VALUES (183, 44402, 'Buckridge', 'Elvis', 17, 18);
INSERT INTO public.employee VALUES (184, 47680, 'Cronin', 'Stanford', 8, 2);
INSERT INTO public.employee VALUES (185, 25387, 'Schmidt', 'Lia', 10, 22);
INSERT INTO public.employee VALUES (186, 55526, 'Fisher', 'Enola', 7, 14);
INSERT INTO public.employee VALUES (187, 14669, 'Cremin', 'Lauriane', 18, 9);
INSERT INTO public.employee VALUES (188, 59029, 'Lueilwitz', 'Gabrielle', 16, 10);
INSERT INTO public.employee VALUES (189, 65548, 'Schuppe', 'Beulah', 6, 23);
INSERT INTO public.employee VALUES (190, 86409, 'Marvin', 'Peter', 14, 11);
INSERT INTO public.employee VALUES (191, 30299, 'Grimes', 'Vickie', 11, 26);
INSERT INTO public.employee VALUES (192, 85943, 'Dibbert', 'Derek', 5, 19);
INSERT INTO public.employee VALUES (193, 40042, 'McCullough', 'Freeda', 17, 22);
INSERT INTO public.employee VALUES (194, 23733, 'Littel', 'Marielle', 19, 28);
INSERT INTO public.employee VALUES (195, 48384, 'Buckridge', 'Ian', 6, 7);
INSERT INTO public.employee VALUES (196, 30243, 'Durgan', 'Hermann', 18, 9);
INSERT INTO public.employee VALUES (197, 27904, 'Klein', 'Colten', 17, 12);
INSERT INTO public.employee VALUES (198, 32750, 'Sauer', 'Flossie', 6, 5);
INSERT INTO public.employee VALUES (199, 21712, 'Hyatt', 'Sister', 4, 27);
INSERT INTO public.employee VALUES (200, 58873, 'Fahey', 'Thalia', 12, 19);
INSERT INTO public.employee VALUES (201, 23776, 'Kreiger', 'Dejuan', 4, 22);
INSERT INTO public.employee VALUES (202, 54681, 'Jaskolski', 'Darrell', 9, 16);
INSERT INTO public.employee VALUES (203, 47002, 'Beatty', 'Wilbert', 4, 15);
INSERT INTO public.employee VALUES (204, 55224, 'Quitzon', 'Maynard', 17, 3);
INSERT INTO public.employee VALUES (205, 69214, 'Schulist', 'Heaven', 20, 11);
INSERT INTO public.employee VALUES (206, 61144, 'Beer', 'Coy', 6, 13);
INSERT INTO public.employee VALUES (207, 97108, 'Toy', 'Coty', 9, 12);
INSERT INTO public.employee VALUES (208, 84848, 'Satterfield', 'Lexus', 11, 17);
INSERT INTO public.employee VALUES (209, 43025, 'Parisian', 'Brady', 19, 1);
INSERT INTO public.employee VALUES (210, 77934, 'Nikolaus', 'Terrence', 2, 12);
INSERT INTO public.employee VALUES (211, 78652, 'Abernathy', 'Georgianna', 8, 24);
INSERT INTO public.employee VALUES (212, 80894, 'Reilly', 'Syble', 8, 25);
INSERT INTO public.employee VALUES (213, 21556, 'Gislason', 'Kari', 7, 19);
INSERT INTO public.employee VALUES (214, 27728, 'MacGyver', 'Albert', 9, 1);
INSERT INTO public.employee VALUES (215, 90600, 'Farrell', 'Giuseppe', 19, 8);
INSERT INTO public.employee VALUES (216, 21298, 'Marks', 'Dallin', 9, 10);
INSERT INTO public.employee VALUES (217, 15863, 'Murray', 'Carroll', 6, 29);
INSERT INTO public.employee VALUES (218, 93885, 'Dibbert', 'Justine', 4, 28);
INSERT INTO public.employee VALUES (219, 27901, 'Marquardt', 'Lucienne', 19, 13);
INSERT INTO public.employee VALUES (220, 69206, 'O Keefe', 'Beatrice', 4, 2);
INSERT INTO public.employee VALUES (221, 37250, 'Veum', 'Zena', 10, 18);
INSERT INTO public.employee VALUES (222, 88337, 'Zulauf', 'Arvilla', 5, 23);
INSERT INTO public.employee VALUES (223, 67359, 'Harber', 'Marcelina', 16, 23);
INSERT INTO public.employee VALUES (224, 83371, 'Stamm', 'Lonnie', 4, 1);
INSERT INTO public.employee VALUES (225, 82376, 'Zboncak', 'Fae', 16, 29);
INSERT INTO public.employee VALUES (226, 28723, 'Kozey', 'Israel', 5, 22);
INSERT INTO public.employee VALUES (227, 97515, 'Hudson', 'Reinhold', 11, 15);
INSERT INTO public.employee VALUES (228, 71679, 'Schneider', 'Stone', 19, 20);
INSERT INTO public.employee VALUES (229, 96890, 'Block', 'Breanna', 17, 27);
INSERT INTO public.employee VALUES (230, 97296, 'Paucek', 'Hilario', 7, 26);
INSERT INTO public.employee VALUES (231, 23737, 'Labadie', 'Nona', 9, 29);
INSERT INTO public.employee VALUES (232, 72376, 'Jones', 'Tyshawn', 10, 10);
INSERT INTO public.employee VALUES (233, 20551, 'Fahey', 'Virgil', 5, 16);
INSERT INTO public.employee VALUES (234, 71262, 'Carter', 'Lionel', 15, 16);
INSERT INTO public.employee VALUES (235, 40276, 'Mertz', 'Arno', 8, 2);
INSERT INTO public.employee VALUES (236, 19602, 'Moen', 'Tamara', 20, 17);
INSERT INTO public.employee VALUES (237, 75303, 'Kassulke', 'Camron', 4, 10);
INSERT INTO public.employee VALUES (238, 55065, 'Heller', 'Ofelia', 3, 26);
INSERT INTO public.employee VALUES (239, 48089, 'O Reilly', 'Jayme', 17, 7);
INSERT INTO public.employee VALUES (240, 66381, 'Haley', 'Norbert', 11, 8);
INSERT INTO public.employee VALUES (241, 86128, 'Robel', 'Nakia', 13, 23);
INSERT INTO public.employee VALUES (242, 90907, 'Hermann', 'Enrico', 2, 7);
INSERT INTO public.employee VALUES (243, 97848, 'Farrell', 'Kelton', 12, 4);
INSERT INTO public.employee VALUES (244, 31319, 'Smitham', 'Martin', 15, 4);
INSERT INTO public.employee VALUES (245, 49237, 'Dibbert', 'Jena', 10, 17);
INSERT INTO public.employee VALUES (246, 41025, 'Dickinson', 'Oran', 19, 9);
INSERT INTO public.employee VALUES (247, 75290, 'Feil', 'Chaim', 10, 8);
INSERT INTO public.employee VALUES (248, 50255, 'McCullough', 'Cortney', 20, 17);
INSERT INTO public.employee VALUES (249, 54185, 'Deckow', 'Sabina', 12, 15);
INSERT INTO public.employee VALUES (250, 99643, 'Sanford', 'Asa', 12, 8);
INSERT INTO public.employee VALUES (251, 67231, 'Crona', 'Bradley', 19, 16);
INSERT INTO public.employee VALUES (252, 67789, 'Kozey', 'Renee', 9, 13);
INSERT INTO public.employee VALUES (253, 96569, 'McCullough', 'Waldo', 12, 18);
INSERT INTO public.employee VALUES (254, 16790, 'Ernser', 'Robyn', 14, 3);
INSERT INTO public.employee VALUES (255, 98183, 'Grady', 'Kenton', 15, 9);
INSERT INTO public.employee VALUES (256, 94276, 'Roob', 'Estella', 12, 19);
INSERT INTO public.employee VALUES (257, 98117, 'Pollich', 'Angelica', 10, 10);
INSERT INTO public.employee VALUES (258, 48003, 'Walsh', 'Carleton', 13, 21);
INSERT INTO public.employee VALUES (259, 52801, 'Swaniawski', 'Emery', 4, 21);
INSERT INTO public.employee VALUES (260, 77221, 'Runolfsson', 'Meagan', 1, 10);
INSERT INTO public.employee VALUES (261, 19350, 'Moen', 'Charley', 18, 5);
INSERT INTO public.employee VALUES (262, 92477, 'Schmitt', 'Carrie', 17, 14);
INSERT INTO public.employee VALUES (263, 32401, 'Funk', 'Johann', 10, 26);
INSERT INTO public.employee VALUES (264, 55370, 'Effertz', 'Shaina', 10, 17);
INSERT INTO public.employee VALUES (265, 49465, 'Stokes', 'Glennie', 10, 18);
INSERT INTO public.employee VALUES (266, 49808, 'Mayert', 'Kane', 7, 22);
INSERT INTO public.employee VALUES (267, 59980, 'Farrell', 'Tyreek', 1, 21);
INSERT INTO public.employee VALUES (268, 39479, 'Ward', 'Kelli', 17, 15);
INSERT INTO public.employee VALUES (269, 63314, 'Hilll', 'Nash', 8, 25);
INSERT INTO public.employee VALUES (270, 10042, 'Bayer', 'Genesis', 13, 25);
INSERT INTO public.employee VALUES (271, 50738, 'Homenick', 'Jerad', 1, 1);
INSERT INTO public.employee VALUES (272, 18548, 'Kris', 'Annalise', 14, 5);
INSERT INTO public.employee VALUES (273, 56827, 'Klein', 'Larissa', 4, 24);
INSERT INTO public.employee VALUES (274, 98944, 'Casper', 'Carmella', 10, 13);
INSERT INTO public.employee VALUES (275, 47034, 'Pacocha', 'Kurt', 4, 1);
INSERT INTO public.employee VALUES (276, 97534, 'Hermann', 'Tiffany', 17, 24);
INSERT INTO public.employee VALUES (277, 40071, 'Blanda', 'Lexi', 7, 8);
INSERT INTO public.employee VALUES (278, 97495, 'Powlowski', 'Justyn', 20, 17);
INSERT INTO public.employee VALUES (279, 67584, 'Koepp', 'Serena', 13, 18);
INSERT INTO public.employee VALUES (280, 55360, 'Gibson', 'Renee', 18, 10);
INSERT INTO public.employee VALUES (281, 71855, 'Baumbach', 'Kiara', 8, 24);
INSERT INTO public.employee VALUES (282, 78213, 'Renner', 'Emmanuel', 11, 25);
INSERT INTO public.employee VALUES (283, 39577, 'Kub', 'Candido', 5, 2);
INSERT INTO public.employee VALUES (284, 36225, 'Bogisich', 'Chance', 13, 18);
INSERT INTO public.employee VALUES (285, 86502, 'Volkman', 'Jeffry', 14, 18);
INSERT INTO public.employee VALUES (286, 47535, 'Ferry', 'Drew', 2, 16);
INSERT INTO public.employee VALUES (287, 47634, 'Quigley', 'Raina', 18, 7);
INSERT INTO public.employee VALUES (288, 17753, 'Maggio', 'Audreanne', 12, 27);
INSERT INTO public.employee VALUES (289, 36000, 'Halvorson', 'Marta', 8, 13);
INSERT INTO public.employee VALUES (290, 61132, 'Lind', 'Jovani', 6, 15);
INSERT INTO public.employee VALUES (291, 15317, 'Prohaska', 'Westley', 12, 15);
INSERT INTO public.employee VALUES (292, 41064, 'Conn', 'Florencio', 19, 8);
INSERT INTO public.employee VALUES (293, 82771, 'Hessel', 'Oscar', 4, 21);
INSERT INTO public.employee VALUES (294, 55996, 'Feil', 'Elmo', 2, 27);
INSERT INTO public.employee VALUES (295, 60039, 'Lueilwitz', 'Keegan', 6, 6);
INSERT INTO public.employee VALUES (296, 39535, 'Jenkins', 'Kadin', 4, 23);
INSERT INTO public.employee VALUES (297, 85866, 'Feeney', 'Garland', 20, 10);
INSERT INTO public.employee VALUES (298, 32931, 'Tromp', 'Dejuan', 1, 17);
INSERT INTO public.employee VALUES (299, 42741, 'Harvey', 'Nick', 20, 22);
INSERT INTO public.employee VALUES (300, 41232, 'Larson', 'Tyler', 19, 10);
INSERT INTO public.employee VALUES (301, 59134, 'Ruecker', 'Casey', 6, 1);
INSERT INTO public.employee VALUES (302, 93985, 'Stroman', 'Jailyn', 18, 28);
INSERT INTO public.employee VALUES (303, 24519, 'King', 'Germaine', 13, 1);
INSERT INTO public.employee VALUES (304, 76283, 'Zulauf', 'Napoleon', 5, 13);


--
-- TOC entry 4917 (class 0 OID 16402)
-- Dependencies: 221
-- Data for Name: employee_position; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.employee_position VALUES (1, 18);
INSERT INTO public.employee_position VALUES (2, 19);
INSERT INTO public.employee_position VALUES (3, 10);
INSERT INTO public.employee_position VALUES (4, 3);
INSERT INTO public.employee_position VALUES (5, 6);
INSERT INTO public.employee_position VALUES (6, 9);
INSERT INTO public.employee_position VALUES (7, 14);
INSERT INTO public.employee_position VALUES (8, 20);
INSERT INTO public.employee_position VALUES (9, 21);
INSERT INTO public.employee_position VALUES (10, 6);
INSERT INTO public.employee_position VALUES (11, 3);
INSERT INTO public.employee_position VALUES (12, 14);
INSERT INTO public.employee_position VALUES (13, 16);
INSERT INTO public.employee_position VALUES (14, 18);
INSERT INTO public.employee_position VALUES (15, 5);
INSERT INTO public.employee_position VALUES (16, 18);
INSERT INTO public.employee_position VALUES (17, 15);
INSERT INTO public.employee_position VALUES (18, 10);
INSERT INTO public.employee_position VALUES (19, 11);
INSERT INTO public.employee_position VALUES (20, 4);
INSERT INTO public.employee_position VALUES (21, 10);
INSERT INTO public.employee_position VALUES (22, 9);
INSERT INTO public.employee_position VALUES (23, 18);
INSERT INTO public.employee_position VALUES (24, 21);
INSERT INTO public.employee_position VALUES (25, 2);
INSERT INTO public.employee_position VALUES (26, 17);
INSERT INTO public.employee_position VALUES (27, 11);
INSERT INTO public.employee_position VALUES (28, 18);
INSERT INTO public.employee_position VALUES (29, 18);
INSERT INTO public.employee_position VALUES (30, 19);
INSERT INTO public.employee_position VALUES (31, 5);
INSERT INTO public.employee_position VALUES (32, 1);
INSERT INTO public.employee_position VALUES (33, 16);
INSERT INTO public.employee_position VALUES (34, 18);
INSERT INTO public.employee_position VALUES (35, 20);
INSERT INTO public.employee_position VALUES (36, 21);
INSERT INTO public.employee_position VALUES (37, 3);
INSERT INTO public.employee_position VALUES (38, 11);
INSERT INTO public.employee_position VALUES (39, 10);
INSERT INTO public.employee_position VALUES (40, 4);
INSERT INTO public.employee_position VALUES (41, 17);
INSERT INTO public.employee_position VALUES (42, 7);
INSERT INTO public.employee_position VALUES (43, 5);
INSERT INTO public.employee_position VALUES (44, 17);
INSERT INTO public.employee_position VALUES (45, 14);
INSERT INTO public.employee_position VALUES (46, 6);
INSERT INTO public.employee_position VALUES (47, 19);
INSERT INTO public.employee_position VALUES (48, 2);
INSERT INTO public.employee_position VALUES (49, 5);
INSERT INTO public.employee_position VALUES (50, 10);
INSERT INTO public.employee_position VALUES (51, 11);
INSERT INTO public.employee_position VALUES (52, 6);
INSERT INTO public.employee_position VALUES (53, 5);
INSERT INTO public.employee_position VALUES (54, 6);
INSERT INTO public.employee_position VALUES (55, 16);
INSERT INTO public.employee_position VALUES (56, 15);
INSERT INTO public.employee_position VALUES (57, 5);
INSERT INTO public.employee_position VALUES (58, 15);
INSERT INTO public.employee_position VALUES (59, 4);
INSERT INTO public.employee_position VALUES (60, 17);
INSERT INTO public.employee_position VALUES (61, 4);
INSERT INTO public.employee_position VALUES (62, 13);
INSERT INTO public.employee_position VALUES (63, 20);
INSERT INTO public.employee_position VALUES (64, 4);
INSERT INTO public.employee_position VALUES (65, 13);
INSERT INTO public.employee_position VALUES (66, 14);
INSERT INTO public.employee_position VALUES (67, 14);
INSERT INTO public.employee_position VALUES (68, 5);
INSERT INTO public.employee_position VALUES (69, 1);
INSERT INTO public.employee_position VALUES (70, 10);
INSERT INTO public.employee_position VALUES (71, 2);
INSERT INTO public.employee_position VALUES (72, 14);
INSERT INTO public.employee_position VALUES (73, 4);
INSERT INTO public.employee_position VALUES (74, 5);
INSERT INTO public.employee_position VALUES (75, 14);
INSERT INTO public.employee_position VALUES (76, 4);
INSERT INTO public.employee_position VALUES (77, 8);
INSERT INTO public.employee_position VALUES (78, 15);
INSERT INTO public.employee_position VALUES (79, 3);
INSERT INTO public.employee_position VALUES (80, 14);
INSERT INTO public.employee_position VALUES (81, 11);
INSERT INTO public.employee_position VALUES (82, 5);
INSERT INTO public.employee_position VALUES (83, 14);
INSERT INTO public.employee_position VALUES (84, 4);
INSERT INTO public.employee_position VALUES (85, 9);
INSERT INTO public.employee_position VALUES (86, 2);
INSERT INTO public.employee_position VALUES (87, 14);
INSERT INTO public.employee_position VALUES (88, 6);
INSERT INTO public.employee_position VALUES (89, 6);
INSERT INTO public.employee_position VALUES (90, 17);
INSERT INTO public.employee_position VALUES (91, 2);
INSERT INTO public.employee_position VALUES (92, 17);
INSERT INTO public.employee_position VALUES (93, 3);
INSERT INTO public.employee_position VALUES (94, 19);
INSERT INTO public.employee_position VALUES (95, 18);
INSERT INTO public.employee_position VALUES (96, 21);
INSERT INTO public.employee_position VALUES (97, 19);
INSERT INTO public.employee_position VALUES (98, 10);
INSERT INTO public.employee_position VALUES (99, 8);
INSERT INTO public.employee_position VALUES (100, 13);
INSERT INTO public.employee_position VALUES (101, 1);
INSERT INTO public.employee_position VALUES (102, 8);
INSERT INTO public.employee_position VALUES (103, 9);
INSERT INTO public.employee_position VALUES (104, 13);
INSERT INTO public.employee_position VALUES (105, 7);
INSERT INTO public.employee_position VALUES (106, 19);
INSERT INTO public.employee_position VALUES (107, 19);
INSERT INTO public.employee_position VALUES (108, 10);
INSERT INTO public.employee_position VALUES (109, 3);
INSERT INTO public.employee_position VALUES (110, 14);
INSERT INTO public.employee_position VALUES (111, 16);
INSERT INTO public.employee_position VALUES (112, 9);
INSERT INTO public.employee_position VALUES (113, 9);
INSERT INTO public.employee_position VALUES (114, 3);
INSERT INTO public.employee_position VALUES (115, 17);
INSERT INTO public.employee_position VALUES (116, 5);
INSERT INTO public.employee_position VALUES (117, 17);
INSERT INTO public.employee_position VALUES (118, 10);
INSERT INTO public.employee_position VALUES (119, 4);
INSERT INTO public.employee_position VALUES (120, 4);
INSERT INTO public.employee_position VALUES (121, 9);
INSERT INTO public.employee_position VALUES (122, 7);
INSERT INTO public.employee_position VALUES (123, 15);
INSERT INTO public.employee_position VALUES (124, 5);
INSERT INTO public.employee_position VALUES (125, 1);
INSERT INTO public.employee_position VALUES (126, 20);
INSERT INTO public.employee_position VALUES (127, 4);
INSERT INTO public.employee_position VALUES (128, 3);
INSERT INTO public.employee_position VALUES (129, 11);
INSERT INTO public.employee_position VALUES (130, 7);
INSERT INTO public.employee_position VALUES (131, 9);
INSERT INTO public.employee_position VALUES (132, 4);
INSERT INTO public.employee_position VALUES (133, 16);
INSERT INTO public.employee_position VALUES (134, 20);
INSERT INTO public.employee_position VALUES (135, 1);
INSERT INTO public.employee_position VALUES (136, 18);
INSERT INTO public.employee_position VALUES (137, 8);
INSERT INTO public.employee_position VALUES (138, 16);
INSERT INTO public.employee_position VALUES (139, 16);
INSERT INTO public.employee_position VALUES (140, 10);
INSERT INTO public.employee_position VALUES (141, 12);
INSERT INTO public.employee_position VALUES (142, 13);
INSERT INTO public.employee_position VALUES (143, 1);
INSERT INTO public.employee_position VALUES (144, 13);
INSERT INTO public.employee_position VALUES (145, 12);
INSERT INTO public.employee_position VALUES (146, 11);
INSERT INTO public.employee_position VALUES (147, 20);
INSERT INTO public.employee_position VALUES (148, 17);
INSERT INTO public.employee_position VALUES (149, 2);
INSERT INTO public.employee_position VALUES (150, 13);
INSERT INTO public.employee_position VALUES (151, 15);
INSERT INTO public.employee_position VALUES (152, 10);
INSERT INTO public.employee_position VALUES (153, 1);
INSERT INTO public.employee_position VALUES (154, 14);
INSERT INTO public.employee_position VALUES (155, 8);
INSERT INTO public.employee_position VALUES (156, 6);
INSERT INTO public.employee_position VALUES (157, 14);
INSERT INTO public.employee_position VALUES (158, 13);
INSERT INTO public.employee_position VALUES (159, 6);
INSERT INTO public.employee_position VALUES (160, 18);
INSERT INTO public.employee_position VALUES (161, 2);
INSERT INTO public.employee_position VALUES (162, 9);
INSERT INTO public.employee_position VALUES (163, 15);
INSERT INTO public.employee_position VALUES (164, 15);
INSERT INTO public.employee_position VALUES (165, 11);
INSERT INTO public.employee_position VALUES (166, 11);
INSERT INTO public.employee_position VALUES (167, 5);
INSERT INTO public.employee_position VALUES (168, 12);
INSERT INTO public.employee_position VALUES (169, 18);
INSERT INTO public.employee_position VALUES (170, 19);
INSERT INTO public.employee_position VALUES (171, 10);
INSERT INTO public.employee_position VALUES (172, 5);
INSERT INTO public.employee_position VALUES (173, 14);
INSERT INTO public.employee_position VALUES (174, 16);
INSERT INTO public.employee_position VALUES (175, 20);
INSERT INTO public.employee_position VALUES (176, 14);
INSERT INTO public.employee_position VALUES (177, 21);
INSERT INTO public.employee_position VALUES (178, 6);
INSERT INTO public.employee_position VALUES (179, 18);
INSERT INTO public.employee_position VALUES (180, 4);
INSERT INTO public.employee_position VALUES (181, 13);
INSERT INTO public.employee_position VALUES (182, 5);
INSERT INTO public.employee_position VALUES (183, 15);
INSERT INTO public.employee_position VALUES (184, 12);
INSERT INTO public.employee_position VALUES (185, 2);
INSERT INTO public.employee_position VALUES (186, 10);
INSERT INTO public.employee_position VALUES (187, 15);
INSERT INTO public.employee_position VALUES (188, 21);
INSERT INTO public.employee_position VALUES (189, 13);
INSERT INTO public.employee_position VALUES (190, 9);
INSERT INTO public.employee_position VALUES (191, 17);
INSERT INTO public.employee_position VALUES (192, 16);
INSERT INTO public.employee_position VALUES (193, 5);
INSERT INTO public.employee_position VALUES (194, 21);
INSERT INTO public.employee_position VALUES (195, 18);
INSERT INTO public.employee_position VALUES (196, 13);
INSERT INTO public.employee_position VALUES (197, 7);
INSERT INTO public.employee_position VALUES (198, 4);
INSERT INTO public.employee_position VALUES (199, 14);
INSERT INTO public.employee_position VALUES (200, 1);
INSERT INTO public.employee_position VALUES (201, 7);
INSERT INTO public.employee_position VALUES (202, 13);
INSERT INTO public.employee_position VALUES (203, 6);
INSERT INTO public.employee_position VALUES (204, 8);
INSERT INTO public.employee_position VALUES (205, 11);
INSERT INTO public.employee_position VALUES (206, 5);
INSERT INTO public.employee_position VALUES (207, 14);
INSERT INTO public.employee_position VALUES (208, 4);
INSERT INTO public.employee_position VALUES (209, 13);
INSERT INTO public.employee_position VALUES (210, 17);
INSERT INTO public.employee_position VALUES (211, 2);
INSERT INTO public.employee_position VALUES (212, 5);
INSERT INTO public.employee_position VALUES (213, 17);
INSERT INTO public.employee_position VALUES (214, 21);
INSERT INTO public.employee_position VALUES (215, 2);
INSERT INTO public.employee_position VALUES (216, 20);
INSERT INTO public.employee_position VALUES (217, 19);
INSERT INTO public.employee_position VALUES (218, 15);
INSERT INTO public.employee_position VALUES (219, 3);
INSERT INTO public.employee_position VALUES (220, 9);
INSERT INTO public.employee_position VALUES (221, 11);
INSERT INTO public.employee_position VALUES (222, 17);
INSERT INTO public.employee_position VALUES (223, 8);
INSERT INTO public.employee_position VALUES (224, 19);
INSERT INTO public.employee_position VALUES (225, 2);
INSERT INTO public.employee_position VALUES (226, 13);
INSERT INTO public.employee_position VALUES (227, 6);
INSERT INTO public.employee_position VALUES (228, 3);
INSERT INTO public.employee_position VALUES (229, 10);
INSERT INTO public.employee_position VALUES (230, 3);
INSERT INTO public.employee_position VALUES (231, 13);
INSERT INTO public.employee_position VALUES (232, 15);
INSERT INTO public.employee_position VALUES (233, 19);
INSERT INTO public.employee_position VALUES (234, 12);
INSERT INTO public.employee_position VALUES (235, 21);
INSERT INTO public.employee_position VALUES (236, 13);
INSERT INTO public.employee_position VALUES (237, 5);
INSERT INTO public.employee_position VALUES (238, 20);
INSERT INTO public.employee_position VALUES (239, 19);
INSERT INTO public.employee_position VALUES (240, 2);
INSERT INTO public.employee_position VALUES (241, 12);
INSERT INTO public.employee_position VALUES (242, 9);
INSERT INTO public.employee_position VALUES (243, 9);
INSERT INTO public.employee_position VALUES (244, 3);
INSERT INTO public.employee_position VALUES (245, 5);
INSERT INTO public.employee_position VALUES (246, 9);
INSERT INTO public.employee_position VALUES (247, 5);
INSERT INTO public.employee_position VALUES (248, 5);
INSERT INTO public.employee_position VALUES (249, 1);
INSERT INTO public.employee_position VALUES (250, 5);
INSERT INTO public.employee_position VALUES (251, 18);
INSERT INTO public.employee_position VALUES (252, 1);
INSERT INTO public.employee_position VALUES (253, 20);
INSERT INTO public.employee_position VALUES (254, 10);
INSERT INTO public.employee_position VALUES (255, 6);
INSERT INTO public.employee_position VALUES (256, 15);
INSERT INTO public.employee_position VALUES (257, 19);
INSERT INTO public.employee_position VALUES (258, 4);
INSERT INTO public.employee_position VALUES (259, 18);
INSERT INTO public.employee_position VALUES (260, 13);
INSERT INTO public.employee_position VALUES (261, 1);
INSERT INTO public.employee_position VALUES (262, 4);
INSERT INTO public.employee_position VALUES (263, 7);
INSERT INTO public.employee_position VALUES (264, 18);
INSERT INTO public.employee_position VALUES (265, 9);
INSERT INTO public.employee_position VALUES (266, 3);
INSERT INTO public.employee_position VALUES (267, 12);
INSERT INTO public.employee_position VALUES (268, 2);
INSERT INTO public.employee_position VALUES (269, 15);
INSERT INTO public.employee_position VALUES (270, 14);
INSERT INTO public.employee_position VALUES (271, 15);
INSERT INTO public.employee_position VALUES (272, 4);
INSERT INTO public.employee_position VALUES (273, 13);
INSERT INTO public.employee_position VALUES (274, 17);
INSERT INTO public.employee_position VALUES (275, 20);
INSERT INTO public.employee_position VALUES (276, 16);
INSERT INTO public.employee_position VALUES (277, 9);
INSERT INTO public.employee_position VALUES (278, 20);
INSERT INTO public.employee_position VALUES (279, 18);
INSERT INTO public.employee_position VALUES (280, 16);
INSERT INTO public.employee_position VALUES (281, 7);
INSERT INTO public.employee_position VALUES (282, 16);
INSERT INTO public.employee_position VALUES (283, 3);
INSERT INTO public.employee_position VALUES (284, 3);
INSERT INTO public.employee_position VALUES (285, 11);
INSERT INTO public.employee_position VALUES (286, 5);
INSERT INTO public.employee_position VALUES (287, 16);
INSERT INTO public.employee_position VALUES (288, 5);
INSERT INTO public.employee_position VALUES (289, 5);
INSERT INTO public.employee_position VALUES (290, 5);
INSERT INTO public.employee_position VALUES (291, 3);
INSERT INTO public.employee_position VALUES (292, 12);
INSERT INTO public.employee_position VALUES (293, 12);
INSERT INTO public.employee_position VALUES (294, 5);
INSERT INTO public.employee_position VALUES (295, 14);
INSERT INTO public.employee_position VALUES (296, 4);
INSERT INTO public.employee_position VALUES (297, 5);
INSERT INTO public.employee_position VALUES (298, 5);
INSERT INTO public.employee_position VALUES (299, 19);
INSERT INTO public.employee_position VALUES (300, 6);
INSERT INTO public.employee_position VALUES (301, 3);
INSERT INTO public.employee_position VALUES (302, 7);
INSERT INTO public.employee_position VALUES (303, 13);
INSERT INTO public.employee_position VALUES (304, 8);


--
-- TOC entry 4918 (class 0 OID 16405)
-- Dependencies: 222
-- Data for Name: logs; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.logs VALUES (1, '2024-02-13 02:40:48.914', '2024-12-01 17:10:39.939', 788, 1, 1, 29, 129);
INSERT INTO public.logs VALUES (2, '2024-08-02 20:05:49.271', '2024-12-01 09:56:50.369', 865, 2, 85, 29, 58);
INSERT INTO public.logs VALUES (3, '2024-01-20 02:08:46.831', '2024-12-01 11:18:25.552', 769, 3, 209, 11, 219);
INSERT INTO public.logs VALUES (4, '2024-10-27 03:59:35.096', '2024-11-30 20:41:10.87', 462, 4, 210, 6, 190);
INSERT INTO public.logs VALUES (5, '2024-07-09 04:14:28.769', '2024-12-01 05:06:58.372', 397, 5, 124, 14, 151);
INSERT INTO public.logs VALUES (6, '2023-11-29 18:52:31.115', '2024-12-01 16:11:24.049', 381, 6, 6, 28, 68);
INSERT INTO public.logs VALUES (7, '2024-01-18 13:30:27.337', '2024-12-01 09:31:17.907', 386, 7, 150, 15, 252);
INSERT INTO public.logs VALUES (8, '2023-12-10 09:56:40.328', '2024-12-01 12:36:17.073', 187, 8, 160, 4, 56);
INSERT INTO public.logs VALUES (9, '2024-11-09 02:22:39.786', '2024-12-01 04:31:29.969', 533, 9, 153, 1, 74);
INSERT INTO public.logs VALUES (10, '2024-01-20 10:29:00.123', '2024-12-01 06:39:58.009', 755, 10, 110, 15, 48);
INSERT INTO public.logs VALUES (11, '2024-06-14 14:19:57.023', '2024-12-01 01:40:41.986', 766, 11, 66, 27, 140);
INSERT INTO public.logs VALUES (12, '2024-08-19 19:22:14.715', '2024-12-01 12:03:17.066', 211, 12, 154, 7, 88);
INSERT INTO public.logs VALUES (13, '2024-03-28 11:59:42.403', '2024-12-01 15:30:43.487', 139, 13, 164, 22, 95);
INSERT INTO public.logs VALUES (14, '2024-11-10 08:00:20.89', '2024-12-01 06:06:31.765', 553, 14, 119, 20, 281);
INSERT INTO public.logs VALUES (15, '2024-03-30 18:19:32.78', '2024-12-01 16:36:29.6', 633, 15, 149, 29, 169);
INSERT INTO public.logs VALUES (16, '2024-07-18 02:24:12.925', '2024-11-30 20:48:30.512', 177, 16, 130, 24, 91);
INSERT INTO public.logs VALUES (17, '2024-10-04 14:40:27.599', '2024-12-01 13:02:43.094', 534, 17, 97, 19, 30);
INSERT INTO public.logs VALUES (18, '2024-10-06 07:57:57.763', '2024-12-01 10:55:23.713', 299, 18, 143, 3, 205);
INSERT INTO public.logs VALUES (19, '2024-10-02 11:37:59.319', '2024-12-01 12:48:08.691', 342, 19, 202, 21, 25);
INSERT INTO public.logs VALUES (20, '2024-09-30 07:21:22.755', '2024-12-01 01:23:33.329', 370, 20, 78, 21, 129);
INSERT INTO public.logs VALUES (21, '2024-04-02 14:01:54.11', '2024-11-30 20:49:16.707', 495, 21, 87, 20, 233);
INSERT INTO public.logs VALUES (22, '2024-03-17 14:04:03.177', '2024-11-30 19:26:23.544', 461, 22, 93, 2, 222);
INSERT INTO public.logs VALUES (23, '2024-03-29 08:03:12.028', '2024-12-01 10:54:05.228', 591, 23, 103, 9, 186);
INSERT INTO public.logs VALUES (24, '2023-12-18 03:16:14.557', '2024-12-01 02:30:26.025', 269, 24, 43, 5, 90);
INSERT INTO public.logs VALUES (25, '2024-04-10 18:27:44.711', '2024-12-01 10:47:30.339', 295, 25, 54, 23, 26);
INSERT INTO public.logs VALUES (26, '2024-05-24 06:14:50.097', '2024-12-01 07:13:58.453', 237, 26, 22, 4, 63);
INSERT INTO public.logs VALUES (27, '2024-11-09 21:43:21.354', '2024-12-01 15:47:08.357', 580, 27, 63, 9, 148);
INSERT INTO public.logs VALUES (28, '2024-07-30 09:52:21.554', '2024-12-01 14:57:25.635', 585, 28, 97, 13, 277);
INSERT INTO public.logs VALUES (29, '2024-08-21 10:37:49.749', '2024-12-01 04:51:04.126', 209, 29, 177, 11, 55);
INSERT INTO public.logs VALUES (30, '2024-02-20 02:29:14.454', '2024-12-01 02:41:38.103', 931, 30, 115, 20, 2);
INSERT INTO public.logs VALUES (31, '2024-09-25 04:28:50.39', '2024-12-01 12:17:09.498', 757, 31, 97, 23, 162);
INSERT INTO public.logs VALUES (32, '2024-05-29 00:07:23.025', '2024-11-30 19:20:44.294', 762, 32, 19, 13, 57);
INSERT INTO public.logs VALUES (33, '2024-06-06 14:59:13.029', '2024-12-01 08:56:06.55', 388, 33, 155, 1, 9);
INSERT INTO public.logs VALUES (34, '2024-03-18 07:40:45.74', '2024-12-01 08:24:59.065', 702, 34, 162, 18, 73);
INSERT INTO public.logs VALUES (35, '2023-12-11 20:35:36.94', '2024-12-01 08:23:49.588', 778, 35, 205, 9, 261);
INSERT INTO public.logs VALUES (36, '2024-05-04 03:25:33.513', '2024-12-01 07:33:46.129', 269, 36, 79, 8, 286);
INSERT INTO public.logs VALUES (37, '2024-10-01 06:37:30.055', '2024-12-01 04:59:25.999', 389, 37, 211, 7, 165);
INSERT INTO public.logs VALUES (38, '2023-12-27 21:17:53.614', '2024-12-01 12:28:50.554', 522, 38, 105, 9, 28);
INSERT INTO public.logs VALUES (39, '2023-12-23 13:08:41.322', '2024-12-01 16:33:39.91', 306, 39, 196, 23, 98);
INSERT INTO public.logs VALUES (40, '2024-05-20 10:03:45.28', '2024-12-01 13:47:34.749', 244, 40, 9, 23, 83);
INSERT INTO public.logs VALUES (41, '2024-01-21 04:13:51.875', '2024-11-30 23:09:23.948', 916, 41, 152, 23, 96);
INSERT INTO public.logs VALUES (42, '2024-06-07 03:30:24.307', '2024-12-01 07:14:50.564', 445, 42, 153, 5, 121);
INSERT INTO public.logs VALUES (43, '2024-11-09 08:09:05.223', '2024-11-30 23:36:17.119', 972, 43, 98, 4, 175);
INSERT INTO public.logs VALUES (44, '2024-08-14 11:30:23.957', '2024-11-30 20:47:38.79', 566, 44, 90, 28, 91);
INSERT INTO public.logs VALUES (45, '2024-11-27 09:07:15.517', '2024-12-01 07:40:46.128', 468, 45, 173, 22, 17);
INSERT INTO public.logs VALUES (46, '2024-03-06 10:44:58.959', '2024-11-30 20:15:42.725', 701, 46, 152, 28, 223);
INSERT INTO public.logs VALUES (47, '2024-10-28 07:22:05.91', '2024-12-01 15:39:42.25', 812, 47, 79, 24, 253);
INSERT INTO public.logs VALUES (48, '2024-03-29 19:03:16.435', '2024-12-01 06:49:14.937', 853, 48, 167, 8, 56);
INSERT INTO public.logs VALUES (49, '2023-12-01 16:54:20.643', '2024-11-30 22:28:35.643', 134, 49, 131, 10, 190);
INSERT INTO public.logs VALUES (50, '2024-05-31 19:41:58.083', '2024-11-30 22:54:36.837', 986, 50, 194, 23, 221);
INSERT INTO public.logs VALUES (51, '2023-12-09 16:34:15.832', '2024-12-01 18:15:27.802', 477, 51, 5, 24, 41);
INSERT INTO public.logs VALUES (52, '2024-05-25 06:41:17.123', '2024-12-01 09:21:42.748', 453, 52, 194, 20, 16);
INSERT INTO public.logs VALUES (53, '2024-03-11 08:13:11.409', '2024-12-01 17:23:23.823', 416, 53, 94, 12, 167);
INSERT INTO public.logs VALUES (54, '2024-09-30 19:21:28.285', '2024-12-01 06:01:42.502', 574, 54, 80, 28, 71);
INSERT INTO public.logs VALUES (55, '2024-05-30 17:44:23.409', '2024-12-01 15:39:09.289', 542, 55, 113, 21, 128);
INSERT INTO public.logs VALUES (56, '2024-10-12 03:20:50.108', '2024-12-01 15:53:44.893', 499, 56, 150, 24, 163);
INSERT INTO public.logs VALUES (57, '2024-10-17 02:18:22.947', '2024-12-01 02:22:54.235', 341, 57, 126, 2, 231);
INSERT INTO public.logs VALUES (58, '2024-07-29 22:12:35.257', '2024-12-01 17:58:14.561', 683, 58, 22, 9, 304);
INSERT INTO public.logs VALUES (59, '2024-01-16 12:30:01.538', '2024-11-30 23:34:16.098', 705, 59, 180, 11, 30);
INSERT INTO public.logs VALUES (60, '2024-05-19 16:37:24.119', '2024-12-01 00:35:49.935', 723, 60, 9, 1, 118);
INSERT INTO public.logs VALUES (61, '2024-07-05 07:59:19.169', '2024-11-30 20:44:35.499', 381, 61, 64, 12, 151);
INSERT INTO public.logs VALUES (62, '2024-11-19 06:06:37.635', '2024-11-30 19:12:24.755', 470, 62, 5, 20, 174);
INSERT INTO public.logs VALUES (63, '2024-09-13 17:20:04.369', '2024-12-01 12:58:28.144', 173, 63, 93, 5, 83);
INSERT INTO public.logs VALUES (64, '2023-12-20 07:40:44.151', '2024-12-01 12:50:50.201', 993, 64, 180, 27, 153);
INSERT INTO public.logs VALUES (65, '2023-12-15 14:28:01.559', '2024-11-30 23:23:57.968', 252, 65, 5, 23, 276);
INSERT INTO public.logs VALUES (66, '2024-01-13 22:27:33.479', '2024-12-01 13:46:26.556', 679, 66, 89, 26, 14);
INSERT INTO public.logs VALUES (67, '2024-07-31 13:38:40.738', '2024-12-01 15:35:59.1', 861, 67, 85, 26, 282);
INSERT INTO public.logs VALUES (68, '2024-04-20 11:21:26.029', '2024-12-01 16:05:19.816', 394, 68, 132, 4, 212);
INSERT INTO public.logs VALUES (69, '2024-10-04 13:26:24.333', '2024-12-01 12:05:16.331', 107, 69, 146, 2, 192);
INSERT INTO public.logs VALUES (70, '2024-05-24 12:05:00.658', '2024-12-01 05:21:17.85', 276, 70, 39, 20, 218);
INSERT INTO public.logs VALUES (71, '2024-09-02 05:48:37.59', '2024-11-30 19:40:38.676', 791, 71, 141, 9, 256);
INSERT INTO public.logs VALUES (72, '2024-03-13 11:23:35.613', '2024-12-01 16:08:52.668', 449, 72, 129, 17, 227);
INSERT INTO public.logs VALUES (73, '2024-03-23 12:50:54.802', '2024-12-01 13:02:54.906', 265, 73, 134, 29, 230);
INSERT INTO public.logs VALUES (74, '2024-05-11 15:38:11.153', '2024-12-01 13:39:22.62', 535, 74, 37, 28, 155);
INSERT INTO public.logs VALUES (75, '2024-03-02 07:16:15.853', '2024-11-30 20:41:29.363', 703, 75, 172, 12, 100);
INSERT INTO public.logs VALUES (76, '2024-09-01 09:55:25.04', '2024-12-01 10:00:46.21', 598, 76, 33, 18, 226);
INSERT INTO public.logs VALUES (77, '2024-05-10 10:52:17.929', '2024-12-01 18:54:50.688', 562, 77, 69, 11, 235);
INSERT INTO public.logs VALUES (78, '2024-03-20 06:46:14.424', '2024-12-01 01:41:00.687', 190, 78, 135, 27, 74);
INSERT INTO public.logs VALUES (79, '2024-01-21 06:18:47.921', '2024-12-01 08:34:52.719', 803, 79, 189, 10, 222);
INSERT INTO public.logs VALUES (80, '2023-12-27 14:32:22.003', '2024-11-30 20:39:59.72', 321, 80, 188, 8, 50);
INSERT INTO public.logs VALUES (81, '2024-04-25 12:20:36.596', '2024-12-01 01:23:24.34', 405, 81, 75, 12, 10);
INSERT INTO public.logs VALUES (82, '2024-08-20 14:41:32.882', '2024-12-01 06:10:52.884', 452, 82, 13, 10, 105);
INSERT INTO public.logs VALUES (83, '2024-02-19 04:21:07.932', '2024-11-30 21:45:26.504', 114, 83, 208, 1, 289);
INSERT INTO public.logs VALUES (84, '2024-07-29 03:34:57.973', '2024-12-01 04:01:32.087', 464, 84, 158, 24, 21);
INSERT INTO public.logs VALUES (85, '2024-01-15 16:43:26.839', '2024-12-01 14:26:55.305', 556, 85, 168, 28, 209);
INSERT INTO public.logs VALUES (86, '2024-01-07 04:49:55.762', '2024-12-01 00:55:23.75', 830, 86, 36, 12, 122);
INSERT INTO public.logs VALUES (87, '2024-04-18 08:19:54.279', '2024-12-01 06:32:35.325', 502, 87, 154, 19, 296);
INSERT INTO public.logs VALUES (88, '2024-11-08 11:15:26.588', '2024-12-01 13:03:55.441', 372, 88, 134, 8, 259);
INSERT INTO public.logs VALUES (89, '2024-04-30 04:27:31.71', '2024-12-01 08:36:44.447', 739, 89, 49, 22, 202);
INSERT INTO public.logs VALUES (90, '2024-07-23 23:39:42.575', '2024-12-01 01:31:37.608', 257, 90, 145, 13, 78);
INSERT INTO public.logs VALUES (91, '2024-05-11 20:11:26.48', '2024-11-30 23:17:39.464', 967, 91, 165, 28, 43);
INSERT INTO public.logs VALUES (92, '2024-03-02 18:33:09.83', '2024-12-01 09:12:21.127', 956, 92, 157, 17, 197);
INSERT INTO public.logs VALUES (93, '2024-05-11 14:24:05.63', '2024-12-01 13:15:31.08', 306, 93, 152, 24, 191);
INSERT INTO public.logs VALUES (94, '2024-10-10 02:43:03.217', '2024-12-01 01:55:28.325', 832, 94, 93, 23, 21);
INSERT INTO public.logs VALUES (95, '2024-05-07 11:53:17.301', '2024-12-01 11:01:35.152', 312, 95, 131, 20, 237);
INSERT INTO public.logs VALUES (96, '2024-06-30 12:29:23.395', '2024-11-30 20:43:01.669', 914, 96, 102, 24, 74);
INSERT INTO public.logs VALUES (97, '2023-12-25 05:16:14.915', '2024-11-30 20:53:45.476', 272, 97, 157, 7, 284);
INSERT INTO public.logs VALUES (98, '2024-10-31 00:19:14.834', '2024-12-01 18:24:08.258', 881, 98, 95, 28, 291);
INSERT INTO public.logs VALUES (99, '2024-10-20 12:01:31.77', '2024-11-30 23:09:01.9', 598, 99, 121, 2, 151);


--
-- TOC entry 4919 (class 0 OID 16410)
-- Dependencies: 223
-- Data for Name: position; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."position" VALUES (1, 'Assistant', 3489, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (2, 'Engineer', 2252, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (3, 'Developer', 4129, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (4, 'Associate', 3305, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (5, 'Specialist', 1224, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (6, 'Director', 3843, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (7, 'Coordinator', 2837, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (8, 'Planner', 2645, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (9, 'Administrator', 7020, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (10, 'Manager', 5318, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (11, 'Architect', 5942, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (12, 'Strategist', 9588, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (13, 'Orchestrator', 1940, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (14, 'Supervisor', 2649, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (15, 'Agent', 7177, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (16, 'Technician', 2303, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (17, 'Consultant', 7318, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (18, 'Producer', 2172, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (19, 'Executive', 5026, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (20, 'Facilitator', 4253, '09:00:00', '18:00:00');
INSERT INTO public."position" VALUES (21, 'Officer', 4144, '09:00:00', '18:00:00');


--
-- TOC entry 4921 (class 0 OID 16416)
-- Dependencies: 225
-- Data for Name: transport; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.transport VALUES (1, 'BMW', '2024-02-19', '2024-12-01', 'E63735W44ANE12297', 12);
INSERT INTO public.transport VALUES (2, 'Mercedes Benz', '2024-02-23', '2024-12-01', 'Y03J2LB1U9RT31941', 16);
INSERT INTO public.transport VALUES (3, 'Polestar', '2024-07-24', '2024-12-01', 'MTLXXP0PP6VE45444', 10);
INSERT INTO public.transport VALUES (4, 'Honda', '2023-12-20', '2024-11-30', 'F7HBREYDETXD71790', 23);
INSERT INTO public.transport VALUES (5, 'Cadillac', '2024-03-29', '2024-12-01', '8KGY16RUZ5KS13272', 25);
INSERT INTO public.transport VALUES (6, 'Maserati', '2024-02-13', '2024-12-01', 'EVWBYGP3RNU379532', 21);
INSERT INTO public.transport VALUES (7, 'Lamborghini', '2023-12-29', '2024-12-01', '90DCPURP5NRR11158', 6);
INSERT INTO public.transport VALUES (8, 'BMW', '2024-10-22', '2024-12-01', '8PDWSXW9WGXE10966', 22);
INSERT INTO public.transport VALUES (9, 'Chrysler', '2024-09-13', '2024-12-01', 'FAP2XD12YNB363535', 5);
INSERT INTO public.transport VALUES (10, 'Porsche', '2024-08-19', '2024-12-01', 'FY0ZC32PTYVS80190', 10);
INSERT INTO public.transport VALUES (11, 'Honda', '2024-11-22', '2024-12-01', 'JKD4VEAHKCTU35061', 19);
INSERT INTO public.transport VALUES (12, 'Dodge', '2024-04-29', '2024-12-01', 'R60BVSLYJBWJ71998', 10);
INSERT INTO public.transport VALUES (13, 'Porsche', '2024-07-31', '2024-12-01', '0HD3ZU0XXGEF96427', 11);
INSERT INTO public.transport VALUES (14, 'Mini', '2024-04-28', '2024-12-01', 'EWBMU5U9J0AV93159', 27);
INSERT INTO public.transport VALUES (15, 'Chrysler', '2023-12-08', '2024-12-01', 'C0R801908ZYK26269', 16);
INSERT INTO public.transport VALUES (16, 'Fiat', '2024-04-25', '2024-12-01', 'TM03JM69WUER84218', 5);
INSERT INTO public.transport VALUES (17, 'Lamborghini', '2024-10-02', '2024-12-01', '215BZSDHFRVR62018', 5);
INSERT INTO public.transport VALUES (18, 'Smart', '2024-01-30', '2024-12-01', '51PTPZEUEXLZ80151', 25);
INSERT INTO public.transport VALUES (19, 'Porsche', '2024-08-10', '2024-12-01', 'GDNWAHE8TSJV62522', 23);
INSERT INTO public.transport VALUES (20, 'Polestar', '2024-04-14', '2024-11-30', '1LTDFAABTVSC99504', 23);
INSERT INTO public.transport VALUES (21, 'Hyundai', '2024-05-18', '2024-12-01', '2ZDGPKR10SR743673', 22);
INSERT INTO public.transport VALUES (22, 'Chevrolet', '2024-08-25', '2024-11-30', 'A1WXZXHSF0XF59227', 19);
INSERT INTO public.transport VALUES (23, 'Toyota', '2024-02-29', '2024-12-01', 'R34HU8P4A4NL14521', 16);
INSERT INTO public.transport VALUES (24, 'Mini', '2024-03-04', '2024-12-01', '9KPZYWL17VD815548', 24);
INSERT INTO public.transport VALUES (25, 'Audi', '2024-09-30', '2024-12-01', 'RXV0V6Y4NDK118866', 1);
INSERT INTO public.transport VALUES (26, 'Porsche', '2024-03-26', '2024-12-01', 'MGG1CAYFZYMA47566', 24);
INSERT INTO public.transport VALUES (27, 'Land Rover', '2023-12-05', '2024-12-01', '0UVECXCE2MNU70305', 10);
INSERT INTO public.transport VALUES (28, 'Ford', '2024-01-12', '2024-12-01', 'KJ73A41PV0WE50132', 17);
INSERT INTO public.transport VALUES (29, 'Honda', '2024-01-25', '2024-12-01', 'X2UAVMEKYJE164029', 4);
INSERT INTO public.transport VALUES (30, 'Aston Martin', '2024-01-24', '2024-12-01', 'EJ4WF2RNV2RE90088', 18);
INSERT INTO public.transport VALUES (31, 'Volkswagen', '2024-02-14', '2024-12-01', 'STYK0BDCB8RD91286', 23);
INSERT INTO public.transport VALUES (32, 'Dodge', '2024-10-08', '2024-11-30', 'Z4ZUF2LV6GH663171', 13);
INSERT INTO public.transport VALUES (33, 'Audi', '2024-06-15', '2024-12-01', 'VL1UKNDX0JZF73038', 18);
INSERT INTO public.transport VALUES (34, 'Lamborghini', '2024-09-20', '2024-12-01', 'GY521NFC07H461962', 16);
INSERT INTO public.transport VALUES (35, 'Polestar', '2024-01-03', '2024-12-01', 'BA30K1A032NJ14934', 19);
INSERT INTO public.transport VALUES (36, 'Volvo', '2024-02-07', '2024-12-01', '6FU4B8P48CRU90991', 9);
INSERT INTO public.transport VALUES (37, 'Honda', '2024-04-29', '2024-12-01', 'U0R4B0V4AJZ811396', 4);
INSERT INTO public.transport VALUES (38, 'Polestar', '2024-06-13', '2024-12-01', 'DDMF2CVBG2EW72684', 16);
INSERT INTO public.transport VALUES (39, 'Fiat', '2024-06-12', '2024-11-30', 'R2N0J3T2PYZ740588', 14);
INSERT INTO public.transport VALUES (40, 'Jaguar', '2024-11-06', '2024-12-01', 'X2R5S4PZEWW967216', 13);
INSERT INTO public.transport VALUES (41, 'Ferrari', '2024-06-17', '2024-12-01', '2H1Z295U0VW235592', 23);
INSERT INTO public.transport VALUES (42, 'Ferrari', '2024-08-07', '2024-12-01', '6MSVUEXBDSRK46463', 20);
INSERT INTO public.transport VALUES (43, 'Bentley', '2024-01-30', '2024-12-01', 'S6APXZUJTDE225080', 12);
INSERT INTO public.transport VALUES (44, 'Cadillac', '2024-02-26', '2024-12-01', 'PP1MZD790KT925004', 2);
INSERT INTO public.transport VALUES (45, 'Mercedes Benz', '2024-05-09', '2024-12-01', 'CLC8PYM27GWD78489', 10);
INSERT INTO public.transport VALUES (46, 'Aston Martin', '2024-03-06', '2024-12-01', 'J5Y6VHF8B5BM46223', 16);
INSERT INTO public.transport VALUES (47, 'Mini', '2024-07-08', '2024-11-30', 'ZYW30AHJ4NRD88096', 20);
INSERT INTO public.transport VALUES (48, 'Land Rover', '2024-06-10', '2024-11-30', 'S8K4PXN1PVF859468', 17);
INSERT INTO public.transport VALUES (49, 'Rolls Royce', '2024-04-28', '2024-11-30', 'JE4CCS7B80T891818', 5);
INSERT INTO public.transport VALUES (50, 'Cadillac', '2024-10-05', '2024-12-01', '6LP6SB5MFEPS39568', 21);
INSERT INTO public.transport VALUES (51, 'Fiat', '2024-06-05', '2024-11-30', 'NLG8HTD8GEX572382', 1);
INSERT INTO public.transport VALUES (52, 'Hyundai', '2024-07-11', '2024-11-30', 'B1JAGPAU58ZK14875', 5);
INSERT INTO public.transport VALUES (53, 'Lamborghini', '2024-04-14', '2024-12-01', 'WV0RTTE3NLBF26182', 22);
INSERT INTO public.transport VALUES (54, 'Lamborghini', '2024-04-29', '2024-12-01', 'EAFDH4D7KTA874566', 20);
INSERT INTO public.transport VALUES (55, 'BMW', '2024-06-02', '2024-12-01', '72HAUTGWPALH94784', 1);
INSERT INTO public.transport VALUES (56, 'Ferrari', '2024-08-02', '2024-12-01', 'UZA59BV5Y2VC65288', 22);
INSERT INTO public.transport VALUES (57, 'Polestar', '2024-11-27', '2024-12-01', '858Z5TC94AEH14793', 10);
INSERT INTO public.transport VALUES (58, 'Cadillac', '2024-11-07', '2024-12-01', 'PRPB1GEZ2SFG55568', 10);
INSERT INTO public.transport VALUES (59, 'Kia', '2024-08-01', '2024-12-01', 'FD8WWB4EE5VH54747', 23);
INSERT INTO public.transport VALUES (60, 'Porsche', '2024-06-03', '2024-12-01', 'D57PJ3GEFTV141058', 24);
INSERT INTO public.transport VALUES (61, 'Jaguar', '2024-10-04', '2024-12-01', 'KNEDBKW9UNUW75046', 15);
INSERT INTO public.transport VALUES (62, 'Toyota', '2024-01-26', '2024-12-01', 'PFNKYZXF6WK536620', 12);
INSERT INTO public.transport VALUES (63, 'Tesla', '2024-05-02', '2024-11-30', '3BNULTZHRWVL81206', 13);
INSERT INTO public.transport VALUES (64, 'Volkswagen', '2024-11-20', '2024-11-30', 'UP5PTETWMDBF25958', 14);
INSERT INTO public.transport VALUES (65, 'Smart', '2024-09-16', '2024-12-01', '8JRGCE0XCHPX84967', 27);
INSERT INTO public.transport VALUES (66, 'Tesla', '2024-03-12', '2024-12-01', 'X031SWEKL6Y116951', 10);
INSERT INTO public.transport VALUES (67, 'Volvo', '2024-02-03', '2024-12-01', '86LAVKDT2SJU26352', 23);
INSERT INTO public.transport VALUES (68, 'Honda', '2024-05-31', '2024-11-30', 'P83D1A0P4SUE30878', 3);
INSERT INTO public.transport VALUES (69, 'Maserati', '2024-01-10', '2024-12-01', 'C7WTNMNFMEL240234', 10);
INSERT INTO public.transport VALUES (70, 'Hyundai', '2024-01-19', '2024-11-30', 'GS2F9R8FCTN810254', 10);
INSERT INTO public.transport VALUES (71, 'Jeep', '2023-12-05', '2024-11-30', 'LWZFYLLY6MW239127', 24);
INSERT INTO public.transport VALUES (72, 'Mini', '2024-06-11', '2024-12-01', 'WWD5TKZ3AHLD35206', 7);
INSERT INTO public.transport VALUES (73, 'Honda', '2024-11-05', '2024-12-01', '0GXAAKXX35VA70268', 5);
INSERT INTO public.transport VALUES (74, 'Jaguar', '2024-08-24', '2024-12-01', 'ZV649UNRN3Y564068', 23);
INSERT INTO public.transport VALUES (75, 'Jaguar', '2024-02-23', '2024-11-30', 'CJ5L0JS3P4U079157', 13);
INSERT INTO public.transport VALUES (76, 'Chrysler', '2024-02-02', '2024-12-01', 'S2VWP8FNZJD177523', 16);
INSERT INTO public.transport VALUES (77, 'Aston Martin', '2024-10-08', '2024-11-30', 'RDH54ZBKG5KG32719', 8);
INSERT INTO public.transport VALUES (78, 'Lamborghini', '2024-07-28', '2024-11-30', 'U7M4ETRX8UV645131', 13);
INSERT INTO public.transport VALUES (79, 'Dodge', '2024-05-31', '2024-12-01', 'X126C7ABA7PV65329', 20);
INSERT INTO public.transport VALUES (80, 'Polestar', '2024-06-07', '2024-12-01', 'DBAKGE0567MM56378', 24);
INSERT INTO public.transport VALUES (81, 'Fiat', '2024-04-18', '2024-12-01', 'HVG754MNFNUZ66392', 18);
INSERT INTO public.transport VALUES (82, 'Cadillac', '2023-12-12', '2024-12-01', 'SNXLNW4704JZ54379', 14);
INSERT INTO public.transport VALUES (83, 'Dodge', '2024-02-21', '2024-12-01', 'ZK1JDB6X6XJ643505', 17);
INSERT INTO public.transport VALUES (84, 'Maserati', '2024-09-07', '2024-11-30', 'RUF95DGL6HU860885', 17);
INSERT INTO public.transport VALUES (85, 'Chevrolet', '2024-11-18', '2024-12-01', 'C6VTML8JXLHH56453', 29);
INSERT INTO public.transport VALUES (86, 'Aston Martin', '2023-12-28', '2024-12-01', '41PSCJJGMHWK82315', 2);
INSERT INTO public.transport VALUES (87, 'Toyota', '2024-11-02', '2024-12-01', 'WYW00TGUVJUZ49906', 1);
INSERT INTO public.transport VALUES (88, 'Jeep', '2024-09-26', '2024-12-01', '76MR9LG8JVDL19029', 6);
INSERT INTO public.transport VALUES (89, 'Aston Martin', '2024-04-28', '2024-12-01', 'UG57DCX9SRWW76160', 26);
INSERT INTO public.transport VALUES (90, 'Bentley', '2024-07-13', '2024-12-01', 'LRXLV450KPW773092', 6);
INSERT INTO public.transport VALUES (91, 'Fiat', '2024-11-05', '2024-12-01', 'EHDU27SNREG473107', 11);
INSERT INTO public.transport VALUES (92, 'Fiat', '2024-09-13', '2024-12-01', 'SW4NN8LBNGVP49196', 4);
INSERT INTO public.transport VALUES (93, 'Honda', '2024-08-14', '2024-11-30', 'RTAH8CWHCHRS29261', 24);
INSERT INTO public.transport VALUES (94, 'Volkswagen', '2024-06-09', '2024-12-01', 'Z68VPFKNFJBZ42308', 5);
INSERT INTO public.transport VALUES (95, 'Tesla', '2024-09-25', '2024-11-30', 'ZPW2G23NV6LW93543', 21);
INSERT INTO public.transport VALUES (96, 'Mercedes Benz', '2024-02-25', '2024-12-01', 'SP26JSYZ6KXF10867', 8);
INSERT INTO public.transport VALUES (97, 'Porsche', '2023-12-29', '2024-12-01', 'LP3JVY8XDGG430374', 5);
INSERT INTO public.transport VALUES (98, 'Cadillac', '2024-06-30', '2024-12-01', 'ZGKB90PWSTBL92088', 9);
INSERT INTO public.transport VALUES (99, 'Polestar', '2024-01-07', '2024-12-01', 'BX7JGT9XSFJG17596', 7);
INSERT INTO public.transport VALUES (100, 'Ford', '2024-09-16', '2024-11-30', '221GGUVS8NBG54320', 11);
INSERT INTO public.transport VALUES (101, 'Mini', '2023-12-28', '2024-12-01', 'LB6552HL5JUD90253', 15);
INSERT INTO public.transport VALUES (102, 'Porsche', '2024-03-21', '2024-12-01', 'AB8UPX3GGNAK62965', 2);
INSERT INTO public.transport VALUES (103, 'Audi', '2024-10-02', '2024-12-01', 'FZ9U5DC7BRFT85582', 27);
INSERT INTO public.transport VALUES (104, 'Smart', '2024-04-14', '2024-12-01', 'PC0S2FY0P4UN22178', 11);
INSERT INTO public.transport VALUES (105, 'Ford', '2024-12-01', '2024-12-01', 'E69GALYLMEWK98400', 14);
INSERT INTO public.transport VALUES (106, 'Mini', '2024-11-15', '2024-12-01', 'AA56C6HPXXB866146', 27);
INSERT INTO public.transport VALUES (107, 'Smart', '2024-05-05', '2024-12-01', 'XR00R91JU3FD13075', 8);
INSERT INTO public.transport VALUES (108, 'Mercedes Benz', '2024-06-22', '2024-12-01', '4ZRPBAENT9R630478', 4);
INSERT INTO public.transport VALUES (109, 'Aston Martin', '2024-03-28', '2024-12-01', 'D30AF5CEGZNL16332', 28);
INSERT INTO public.transport VALUES (110, 'Porsche', '2024-03-02', '2024-12-01', 'ZY9KHL1VLFE091446', 5);
INSERT INTO public.transport VALUES (111, 'Polestar', '2023-12-22', '2024-12-01', '28T78RDEBLUY14394', 17);
INSERT INTO public.transport VALUES (112, 'Kia', '2023-12-23', '2024-11-30', 'JH5DBY6M3LTH61922', 2);
INSERT INTO public.transport VALUES (113, 'Kia', '2024-06-06', '2024-12-01', 'LLRRL4JZNEEF23425', 9);
INSERT INTO public.transport VALUES (114, 'Jaguar', '2024-10-14', '2024-11-30', 'D79NVU5F80HU28497', 5);
INSERT INTO public.transport VALUES (115, 'BMW', '2024-09-30', '2024-12-01', '9L31ZBEVMUKJ96464', 5);
INSERT INTO public.transport VALUES (116, 'Ford', '2024-08-13', '2024-11-30', 'AK0VGLNZ8VX059698', 14);
INSERT INTO public.transport VALUES (117, 'Dodge', '2024-03-27', '2024-11-30', 'LHGAC6SWMCL150744', 27);
INSERT INTO public.transport VALUES (118, 'Audi', '2024-08-21', '2024-12-01', 'CEPYSRHLF5L636584', 17);
INSERT INTO public.transport VALUES (119, 'Honda', '2024-09-13', '2024-12-01', 'JAZHY7EWDSPF96741', 25);
INSERT INTO public.transport VALUES (120, 'Toyota', '2024-04-23', '2024-12-01', 'R8C6LPTMK6RP72991', 18);
INSERT INTO public.transport VALUES (121, 'Jaguar', '2024-11-06', '2024-12-01', 'N28XGLJ8GCKC27217', 24);
INSERT INTO public.transport VALUES (122, 'Porsche', '2024-07-18', '2024-11-30', 'H4HDGARVRWLK66652', 7);
INSERT INTO public.transport VALUES (123, 'Volkswagen', '2024-05-09', '2024-12-01', '895L3H3UJXPL36741', 20);
INSERT INTO public.transport VALUES (124, 'Mercedes Benz', '2024-04-03', '2024-12-01', 'YSD2FU0E3HSN82638', 17);
INSERT INTO public.transport VALUES (125, 'Bentley', '2024-07-15', '2024-12-01', '0X0JGHN2T9WG37447', 1);
INSERT INTO public.transport VALUES (126, 'Bentley', '2024-10-10', '2024-12-01', 'VG2DARH78RVR68871', 22);
INSERT INTO public.transport VALUES (127, 'Chrysler', '2024-02-04', '2024-11-30', 'GHM2VEEZ92FS29578', 14);
INSERT INTO public.transport VALUES (128, 'BMW', '2023-12-08', '2024-12-01', '5GJEK7W82AGR83752', 3);
INSERT INTO public.transport VALUES (129, 'Toyota', '2024-01-20', '2024-12-01', '7JBN1EWTL6XM33894', 21);
INSERT INTO public.transport VALUES (130, 'Chrysler', '2024-01-26', '2024-12-01', 'UMX269RA96CR44206', 6);
INSERT INTO public.transport VALUES (131, 'Mazda', '2024-01-29', '2024-12-01', '9NLCH5Z1XJEB59380', 27);
INSERT INTO public.transport VALUES (132, 'Bentley', '2024-05-17', '2024-12-01', '9U13RFFMDLN917539', 23);
INSERT INTO public.transport VALUES (133, 'Rolls Royce', '2024-11-22', '2024-12-01', 'P5UJAN51WFWG54054', 20);
INSERT INTO public.transport VALUES (134, 'Aston Martin', '2024-04-19', '2024-12-01', 'TDMV6LJ895L267193', 9);
INSERT INTO public.transport VALUES (135, 'Aston Martin', '2024-07-04', '2024-11-30', 'Y8SZJTZSA1FU13569', 24);
INSERT INTO public.transport VALUES (136, 'Smart', '2024-10-04', '2024-12-01', '79CUGAJDPVMN82350', 24);
INSERT INTO public.transport VALUES (137, 'Lamborghini', '2024-08-29', '2024-12-01', '9EHR09ZXRZKM97459', 18);
INSERT INTO public.transport VALUES (138, 'Rolls Royce', '2024-10-20', '2024-12-01', '4CSF7U6KYUMR91839', 19);
INSERT INTO public.transport VALUES (139, 'Jeep', '2024-05-01', '2024-12-01', '7V70TLF82JEA28975', 13);
INSERT INTO public.transport VALUES (140, 'Mazda', '2024-07-13', '2024-12-01', '5D55PM6YLZC932125', 3);
INSERT INTO public.transport VALUES (141, 'Ferrari', '2024-11-30', '2024-12-01', '3ZD1FRVLMLEA81503', 15);
INSERT INTO public.transport VALUES (142, 'Rolls Royce', '2024-08-01', '2024-11-30', '14UEC6RM2FV122376', 10);
INSERT INTO public.transport VALUES (143, 'Porsche', '2024-07-02', '2024-12-01', '9UESSBARVSRH64338', 13);
INSERT INTO public.transport VALUES (144, 'Hyundai', '2024-07-16', '2024-12-01', 'SZCZ1MB9XRFA52503', 25);
INSERT INTO public.transport VALUES (145, 'Tesla', '2024-10-12', '2024-12-01', 'RCJF56E9KVJY86948', 29);
INSERT INTO public.transport VALUES (146, 'Ferrari', '2024-07-16', '2024-11-30', 'AHSJMTMJX8RE88455', 20);
INSERT INTO public.transport VALUES (147, 'Chrysler', '2024-05-18', '2024-11-30', '78DPY8FVPMYY20917', 11);
INSERT INTO public.transport VALUES (148, 'Dodge', '2023-12-19', '2024-12-01', '5D5W0PEJKRAY26304', 2);
INSERT INTO public.transport VALUES (149, 'Tesla', '2024-10-08', '2024-12-01', 'YF4NBPKWWPZ672298', 13);
INSERT INTO public.transport VALUES (150, 'Porsche', '2024-08-26', '2024-12-01', '0R53LU93KFLS63932', 8);
INSERT INTO public.transport VALUES (151, 'Aston Martin', '2024-03-16', '2024-12-01', 'BGL58M1ZCTN753093', 21);
INSERT INTO public.transport VALUES (152, 'Fiat', '2024-03-24', '2024-12-01', 'RXD9EFH4UPHH31744', 1);
INSERT INTO public.transport VALUES (153, 'Aston Martin', '2024-04-30', '2024-12-01', 'DS3FL0JYCPJT13637', 26);
INSERT INTO public.transport VALUES (154, 'Bugatti', '2024-11-27', '2024-11-30', 'T3U5KAYN1NXC93617', 25);
INSERT INTO public.transport VALUES (155, 'Audi', '2024-06-27', '2024-11-30', 'DFPUHYE2DZR884734', 10);
INSERT INTO public.transport VALUES (156, 'Honda', '2024-02-29', '2024-12-01', 'LRKRD649ZXWA86462', 21);
INSERT INTO public.transport VALUES (157, 'Porsche', '2024-06-30', '2024-11-30', 'LWDZTLAV4GCN43947', 20);
INSERT INTO public.transport VALUES (158, 'Bentley', '2024-05-15', '2024-12-01', 'CSBK8VT07GBW21697', 9);
INSERT INTO public.transport VALUES (159, 'Honda', '2024-06-07', '2024-11-30', 'VC8JSG78EBVN90982', 3);
INSERT INTO public.transport VALUES (160, 'Bugatti', '2024-09-13', '2024-11-30', 'DLJC9B835ZJF12259', 28);
INSERT INTO public.transport VALUES (161, 'BMW', '2024-10-01', '2024-11-30', 'XSZNYFFNAZP336768', 8);
INSERT INTO public.transport VALUES (162, 'Rolls Royce', '2024-01-16', '2024-12-01', '17Y8FBM29XWD56863', 9);
INSERT INTO public.transport VALUES (163, 'Hyundai', '2024-08-10', '2024-12-01', '1ZZ0K5ER7VSM19997', 28);
INSERT INTO public.transport VALUES (164, 'Dodge', '2024-05-10', '2024-11-30', 'BW8WW5HDSCEA12132', 15);
INSERT INTO public.transport VALUES (165, 'Bugatti', '2024-03-28', '2024-12-01', '82BCZ43UFNJV25434', 5);
INSERT INTO public.transport VALUES (166, 'Mazda', '2024-08-15', '2024-11-30', 'HCWUJSKTA7T264101', 21);
INSERT INTO public.transport VALUES (167, 'Tesla', '2024-02-06', '2024-11-30', '4EB8BS473KVM52134', 8);
INSERT INTO public.transport VALUES (168, 'Volvo', '2024-08-31', '2024-12-01', 'GCM89Y3DD5MF75340', 13);
INSERT INTO public.transport VALUES (169, 'Mini', '2024-11-19', '2024-12-01', 'C1CABMK31EMW58921', 8);
INSERT INTO public.transport VALUES (170, 'Nissan', '2024-02-09', '2024-12-01', 'HGRXLZL7Y6J449949', 9);
INSERT INTO public.transport VALUES (171, 'BMW', '2024-08-15', '2024-12-01', 'H62X6R5C8RX920292', 10);
INSERT INTO public.transport VALUES (172, 'Maserati', '2024-02-14', '2024-12-01', 'KJ610L24R6MC45573', 8);
INSERT INTO public.transport VALUES (173, 'Cadillac', '2024-07-05', '2024-11-30', 'YBHCW5S6GSBU33995', 7);
INSERT INTO public.transport VALUES (174, 'Smart', '2023-12-21', '2024-12-01', 'KDM7L7B7GHFZ40905', 18);
INSERT INTO public.transport VALUES (175, 'Ferrari', '2024-07-13', '2024-12-01', 'VWCP2T8ET2LY55339', 8);
INSERT INTO public.transport VALUES (176, 'Cadillac', '2024-08-09', '2024-12-01', 'BU6CGBCZDDWD25018', 14);
INSERT INTO public.transport VALUES (177, 'Fiat', '2024-04-18', '2024-12-01', 'WPFVDLP03CLM70333', 21);
INSERT INTO public.transport VALUES (178, 'Volkswagen', '2024-07-01', '2024-12-01', '55VE0R0W3TCN30601', 10);
INSERT INTO public.transport VALUES (179, 'Dodge', '2024-05-03', '2024-12-01', '1L9PHCEZBYRP54150', 22);
INSERT INTO public.transport VALUES (180, 'Aston Martin', '2024-10-29', '2024-12-01', 'F7144RBUN7A869426', 10);
INSERT INTO public.transport VALUES (181, 'Toyota', '2024-05-04', '2024-12-01', '9V24WDDGXGZP29186', 16);
INSERT INTO public.transport VALUES (182, 'Nissan', '2024-07-28', '2024-11-30', 'FAE3GT6HCKG647781', 2);
INSERT INTO public.transport VALUES (183, 'Aston Martin', '2024-09-07', '2024-12-01', '0A3KNT6D41LA29196', 24);
INSERT INTO public.transport VALUES (184, 'Porsche', '2024-08-10', '2024-12-01', 'NBZ8AUNNNXNH33848', 4);
INSERT INTO public.transport VALUES (185, 'Kia', '2024-06-22', '2024-12-01', 'TDPEWZE6WRZK67280', 27);
INSERT INTO public.transport VALUES (186, 'Nissan', '2023-12-10', '2024-12-01', 'M9ZJ2Y0Y21MR91801', 26);
INSERT INTO public.transport VALUES (187, 'Audi', '2024-02-09', '2024-12-01', 'N708E460LVFB17924', 15);
INSERT INTO public.transport VALUES (188, 'Dodge', '2024-01-31', '2024-12-01', 'M48NMUCB69F595404', 13);
INSERT INTO public.transport VALUES (189, 'BMW', '2024-08-12', '2024-12-01', 'HR4EK9RHVUYH74770', 29);
INSERT INTO public.transport VALUES (190, 'Bentley', '2024-08-10', '2024-12-01', 'US4G6F8B96K913731', 29);
INSERT INTO public.transport VALUES (191, 'Cadillac', '2024-09-03', '2024-12-01', 'UAFD685D5PF288858', 2);
INSERT INTO public.transport VALUES (192, 'Audi', '2024-01-17', '2024-12-01', 'NFH1WV25M8MC65491', 4);
INSERT INTO public.transport VALUES (193, 'Kia', '2024-02-14', '2024-11-30', '7ZUEYTE60TNT37146', 13);
INSERT INTO public.transport VALUES (194, 'Chrysler', '2024-11-08', '2024-11-30', '4D3FSAVANUUT51643', 26);
INSERT INTO public.transport VALUES (195, 'Bugatti', '2024-08-13', '2024-12-01', 'FAS0B1BJ4CHC66168', 10);
INSERT INTO public.transport VALUES (196, 'Audi', '2024-07-02', '2024-12-01', 'HEDV02DJVULS10335', 29);
INSERT INTO public.transport VALUES (197, 'Rolls Royce', '2023-12-24', '2024-11-30', 'AZZ2G9DSFVZA51551', 12);
INSERT INTO public.transport VALUES (198, 'Ford', '2024-01-09', '2024-12-01', 'CEVA55060HU232127', 19);
INSERT INTO public.transport VALUES (199, 'Cadillac', '2024-10-18', '2024-12-01', 'MTEZ8C0NT6DG23282', 24);
INSERT INTO public.transport VALUES (200, 'Land Rover', '2024-03-07', '2024-11-30', 'TY214ZKGTPST26031', 11);
INSERT INTO public.transport VALUES (201, 'Nissan', '2024-04-30', '2024-12-01', 'J58NTUPX5WH615987', 23);
INSERT INTO public.transport VALUES (202, 'Mini', '2024-10-11', '2024-12-01', 'EJC8KF0MZ5W312393', 19);
INSERT INTO public.transport VALUES (203, 'Volvo', '2024-11-24', '2024-12-01', 'XRTZJVHMLUV876783', 12);
INSERT INTO public.transport VALUES (204, 'Cadillac', '2024-05-12', '2024-12-01', '47Y3BXDDHDAV96368', 2);
INSERT INTO public.transport VALUES (205, 'Volvo', '2024-02-15', '2024-12-01', 'WUAWN24ZC2XM70871', 16);
INSERT INTO public.transport VALUES (206, 'Ford', '2024-10-05', '2024-12-01', '8NU9MCRRJSLA50620', 9);
INSERT INTO public.transport VALUES (207, 'Mercedes Benz', '2024-08-12', '2024-12-01', 'J0BKFYKKEPVY51723', 11);
INSERT INTO public.transport VALUES (208, 'Aston Martin', '2024-04-04', '2024-12-01', 'A5J99BW2X9NH15995', 15);
INSERT INTO public.transport VALUES (209, 'Chrysler', '2024-05-31', '2024-12-01', 'XZJRE46FU2P249652', 4);
INSERT INTO public.transport VALUES (210, 'Aston Martin', '2024-11-03', '2024-12-01', '3B65A5VV71KD71131', 27);
INSERT INTO public.transport VALUES (211, 'Toyota', '2024-04-18', '2024-12-01', '89Y62FU9ALU915266', 25);


--
-- TOC entry 4929 (class 0 OID 0)
-- Dependencies: 220
-- Name: employee_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.employee_id_seq', 1, false);


--
-- TOC entry 4930 (class 0 OID 0)
-- Dependencies: 224
-- Name: position_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.position_id_seq', 1, false);


--
-- TOC entry 4727 (class 2606 OID 16422)
-- Name: carriers carriers_inn_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.carriers
    ADD CONSTRAINT carriers_inn_key UNIQUE (inn);


--
-- TOC entry 4729 (class 2606 OID 16424)
-- Name: carriers carriers_name_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.carriers
    ADD CONSTRAINT carriers_name_key UNIQUE (company_name);


--
-- TOC entry 4731 (class 2606 OID 16426)
-- Name: carriers carriers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.carriers
    ADD CONSTRAINT carriers_pkey PRIMARY KEY (id);


--
-- TOC entry 4733 (class 2606 OID 16428)
-- Name: documents contract_unique; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.documents
    ADD CONSTRAINT contract_unique UNIQUE (contract);


--
-- TOC entry 4749 (class 2606 OID 16430)
-- Name: logs docs_unique; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.logs
    ADD CONSTRAINT docs_unique UNIQUE (docs_set);


--
-- TOC entry 4735 (class 2606 OID 16432)
-- Name: documents documents_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.documents
    ADD CONSTRAINT documents_pkey PRIMARY KEY (id);


--
-- TOC entry 4741 (class 2606 OID 16434)
-- Name: employee employee_badge_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee
    ADD CONSTRAINT employee_badge_key UNIQUE (badge);


--
-- TOC entry 4743 (class 2606 OID 16436)
-- Name: employee employee_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee
    ADD CONSTRAINT employee_pkey PRIMARY KEY (id);


--
-- TOC entry 4745 (class 2606 OID 16438)
-- Name: employee_position employee_position_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee_position
    ADD CONSTRAINT employee_position_pkey PRIMARY KEY (employee_id, position_id);


--
-- TOC entry 4751 (class 2606 OID 16440)
-- Name: logs logs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.logs
    ADD CONSTRAINT logs_pkey PRIMARY KEY (id);


--
-- TOC entry 4737 (class 2606 OID 16442)
-- Name: documents pay_unique; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.documents
    ADD CONSTRAINT pay_unique UNIQUE (payment);


--
-- TOC entry 4753 (class 2606 OID 16444)
-- Name: position position_name_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."position"
    ADD CONSTRAINT position_name_key UNIQUE (name);


--
-- TOC entry 4755 (class 2606 OID 16446)
-- Name: position position_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."position"
    ADD CONSTRAINT position_pkey PRIMARY KEY (id);


--
-- TOC entry 4757 (class 2606 OID 16448)
-- Name: transport transport_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.transport
    ADD CONSTRAINT transport_pkey PRIMARY KEY (id);


--
-- TOC entry 4759 (class 2606 OID 16450)
-- Name: transport transport_vin_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.transport
    ADD CONSTRAINT transport_vin_key UNIQUE (vin);


--
-- TOC entry 4739 (class 2606 OID 16452)
-- Name: documents ttn_unique; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.documents
    ADD CONSTRAINT ttn_unique UNIQUE (ttn);


--
-- TOC entry 4746 (class 1259 OID 16453)
-- Name: fki_employee_fkey; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX fki_employee_fkey ON public.employee_position USING btree (employee_id);


--
-- TOC entry 4747 (class 1259 OID 16454)
-- Name: fki_o; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX fki_o ON public.employee_position USING btree (position_id);


--
-- TOC entry 4763 (class 2606 OID 16455)
-- Name: logs carrier_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.logs
    ADD CONSTRAINT carrier_id_fk FOREIGN KEY (carrier_id) REFERENCES public.carriers(id);


--
-- TOC entry 4767 (class 2606 OID 16460)
-- Name: transport company_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.transport
    ADD CONSTRAINT company_id_fk FOREIGN KEY (company_id) REFERENCES public.carriers(id);


--
-- TOC entry 4764 (class 2606 OID 16465)
-- Name: logs docs_set_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.logs
    ADD CONSTRAINT docs_set_fk FOREIGN KEY (docs_set) REFERENCES public.documents(id);


--
-- TOC entry 4761 (class 2606 OID 16470)
-- Name: employee_position employee_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee_position
    ADD CONSTRAINT employee_fkey FOREIGN KEY (employee_id) REFERENCES public.employee(id) NOT VALID;


--
-- TOC entry 4760 (class 2606 OID 16475)
-- Name: employee fk_carrierid; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee
    ADD CONSTRAINT fk_carrierid FOREIGN KEY (carrier_id) REFERENCES public.carriers(id);


--
-- TOC entry 4765 (class 2606 OID 16480)
-- Name: logs fk_employee; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.logs
    ADD CONSTRAINT fk_employee FOREIGN KEY (employee_id) REFERENCES public.employee(id);


--
-- TOC entry 4762 (class 2606 OID 16485)
-- Name: employee_position position_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee_position
    ADD CONSTRAINT position_fkey FOREIGN KEY (position_id) REFERENCES public."position"(id) NOT VALID;


--
-- TOC entry 4766 (class 2606 OID 16490)
-- Name: logs transport_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.logs
    ADD CONSTRAINT transport_id_fk FOREIGN KEY (transport_id) REFERENCES public.transport(id);


-- Completed on 2024-12-08 23:25:52

--
-- PostgreSQL database dump complete
--

