xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 267;
 9.14845;3.30529;4.59802;,
 6.26933;3.30529;4.59802;,
 6.26933;-2.00513;6.43118;,
 9.14845;-2.00513;6.43118;,
 6.26933;-7.31554;4.59802;,
 9.14845;-7.31554;4.59802;,
 6.26933;-9.51517;0.17238;,
 9.14845;-9.51517;0.17238;,
 6.26933;-7.31554;-4.25327;,
 9.14845;-7.31554;-4.25327;,
 6.26933;-2.00513;-6.08643;,
 9.14845;-2.00513;-6.08643;,
 6.26933;3.30529;-4.25327;,
 9.14845;3.30529;-4.25327;,
 6.26933;5.50493;0.17238;,
 9.14845;5.50493;0.17238;,
 9.14845;-2.00513;6.43118;,
 9.14845;-2.00513;0.17238;,
 9.14845;3.30529;4.59802;,
 9.14845;-7.31554;4.59802;,
 9.14845;-9.51517;0.17238;,
 9.14845;-7.31554;-4.25327;,
 9.14845;-2.00513;-6.08643;,
 9.14845;3.30529;-4.25327;,
 9.14845;5.50493;0.17238;,
 6.26933;3.30529;4.59802;,
 6.26933;-2.00513;0.17238;,
 6.26933;-2.00513;6.43118;,
 6.26933;-7.31554;4.59802;,
 6.26933;-9.51517;0.17238;,
 6.26933;-7.31554;-4.25327;,
 6.26933;-2.00513;-6.08643;,
 6.26933;3.30529;-4.25327;,
 6.26933;5.50493;0.17238;,
 -6.04933;3.30529;4.59802;,
 -8.92846;3.30529;4.59802;,
 -8.92846;-2.00513;6.43118;,
 -6.04933;-2.00513;6.43118;,
 -8.92846;-7.31554;4.59802;,
 -6.04933;-7.31554;4.59802;,
 -8.92846;-9.51517;0.17238;,
 -6.04933;-9.51517;0.17238;,
 -8.92846;-7.31554;-4.25327;,
 -6.04933;-7.31554;-4.25327;,
 -8.92846;-2.00513;-6.08643;,
 -6.04933;-2.00513;-6.08643;,
 -8.92846;3.30529;-4.25327;,
 -6.04933;3.30529;-4.25327;,
 -8.92846;5.50493;0.17238;,
 -6.04933;5.50493;0.17238;,
 -6.04933;-2.00513;6.43118;,
 -6.04933;-2.00513;0.17238;,
 -6.04933;3.30529;4.59802;,
 -6.04933;-7.31554;4.59802;,
 -6.04933;-9.51517;0.17238;,
 -6.04933;-7.31554;-4.25327;,
 -6.04933;-2.00513;-6.08643;,
 -6.04933;3.30529;-4.25327;,
 -6.04933;5.50493;0.17238;,
 -8.92846;3.30529;4.59802;,
 -8.92846;-2.00513;0.17238;,
 -8.92846;-2.00513;6.43118;,
 -8.92846;-7.31554;4.59802;,
 -8.92846;-9.51517;0.17238;,
 -8.92846;-7.31554;-4.25327;,
 -8.92846;-2.00513;-6.08643;,
 -8.92846;3.30529;-4.25327;,
 -8.92846;5.50493;0.17238;,
 13.99569;-42.45860;13.40578;,
 7.05460;-8.79248;15.13761;,
 -7.11299;-8.79248;15.13761;,
 -14.59525;-41.67225;14.37300;,
 10.09007;-8.79248;-9.41012;,
 10.09007;-8.79248;9.88447;,
 15.45764;-42.49273;8.85771;,
 15.45764;-42.49273;-7.84720;,
 -22.18049;-94.18711;7.87570;,
 -22.18049;-94.18711;-24.25889;,
 -15.53578;-94.18711;16.62465;,
 -17.38330;-94.18711;-29.36319;,
 -8.25404;-94.18711;-42.26033;,
 15.47738;-94.18711;16.62465;,
 6.68821;-94.18711;-42.26033;,
 16.80947;-94.18711;-28.56016;,
 22.12213;-94.18711;7.87570;,
 22.12213;-94.18711;-24.25889;,
 -10.14846;-8.79248;9.88447;,
 -14.85818;-41.67225;9.47808;,
 -14.59525;-41.67225;14.37300;,
 -7.11299;-8.79248;15.13761;,
 7.05460;-8.79248;15.13761;,
 13.99569;-42.45860;13.40578;,
 -10.14846;-8.79248;-9.41012;,
 -14.85818;-41.67225;-8.50080;,
 -10.14846;-8.79248;-9.41012;,
 -10.14846;-8.79248;9.88447;,
 10.09007;-8.79248;-9.41012;,
 -7.11299;-8.79248;15.13761;,
 7.05460;-8.79248;15.13761;,
 10.09007;-8.79248;9.88447;,
 10.09007;-8.79248;-9.41012;,
 10.09007;13.70751;-18.59269;,
 -10.14846;13.70751;-18.59269;,
 -10.14846;-8.79248;-9.41012;,
 -10.14846;15.46218;-22.23060;,
 -10.14846;13.70751;-18.59269;,
 -9.38794;16.21406;-23.78948;,
 10.09007;13.70751;-18.59269;,
 9.32957;16.21406;-23.78948;,
 10.09007;15.46218;-22.23060;,
 -12.97330;-41.55471;-17.03839;,
 -10.14846;-8.51013;-14.16007;,
 -7.04848;-8.79248;-17.38745;,
 -7.15658;-42.72951;-25.90426;,
 -10.14846;15.46218;-22.23060;,
 -9.38794;16.21406;-23.78948;,
 6.68843;-42.72951;-25.90426;,
 6.58033;-8.79248;-17.38745;,
 10.09007;-8.51014;-14.16007;,
 12.43359;-42.34107;-16.88928;,
 9.32957;16.21406;-23.78948;,
 10.09007;15.46218;-22.23060;,
 -7.04848;-8.79248;-17.38745;,
 6.58033;-8.79248;-17.38745;,
 6.68843;-42.72951;-25.90426;,
 -7.15658;-42.72951;-25.90426;,
 -9.38794;16.21406;-23.78948;,
 9.32957;16.21406;-23.78948;,
 10.09007;13.70751;-18.59269;,
 -10.14846;13.70751;-18.59269;,
 -19.62283;-75.84383;-18.75464;,
 -19.62283;-75.84383;8.43541;,
 -15.84289;-75.80277;-25.05817;,
 -7.87068;-76.21312;-32.28404;,
 6.68828;-76.21312;-32.28404;,
 -7.87068;-76.21312;-32.28404;,
 6.68828;-76.21312;-32.28404;,
 15.28099;-76.07744;-24.48356;,
 19.79424;-76.13042;-18.52634;,
 19.79424;-76.13042;8.21872;,
 14.95982;-76.11849;15.50032;,
 14.95982;-76.11849;15.50032;,
 -15.20724;-75.84383;15.83817;,
 -15.20724;-75.84383;15.83817;,
 -22.18049;-94.18711;7.87570;,
 -15.53578;-94.18711;16.62465;,
 -22.18049;-94.18711;-24.25889;,
 -17.38330;-94.18711;-29.36319;,
 -8.25404;-94.18711;-42.26033;,
 6.68821;-94.18711;-42.26033;,
 -8.25404;-94.18711;-42.26033;,
 6.68821;-94.18711;-42.26033;,
 16.80947;-94.18711;-28.56016;,
 22.12213;-94.18711;-24.25889;,
 22.12213;-94.18711;7.87570;,
 15.47738;-94.18711;16.62465;,
 -15.53578;-94.18711;16.62465;,
 15.47738;-94.18711;16.62465;,
 2.19646;-8.95288;3.63108;,
 4.05064;-6.50069;6.58288;,
 0.00930;-6.50069;9.24780;,
 0.00930;-8.95288;5.07332;,
 -4.03206;-6.50069;6.58288;,
 -2.17787;-8.95288;3.63108;,
 -2.17787;-8.95288;3.63108;,
 -4.03206;-6.50069;6.58288;,
 -5.70604;-6.50069;0.14917;,
 -3.08383;-8.95288;0.14917;,
 -4.03206;-6.50069;-6.28455;,
 -2.17787;-8.95288;-3.33273;,
 -2.17787;-8.95288;-3.33273;,
 -4.03206;-6.50069;-6.28455;,
 0.00930;-6.50069;-8.94947;,
 0.00930;-8.95288;-4.77497;,
 4.05064;-6.50069;-6.28455;,
 2.19646;-8.95288;-3.33273;,
 2.19646;-8.95288;-3.33273;,
 4.05064;-6.50069;-6.28455;,
 5.72461;-6.50069;0.14917;,
 3.10241;-8.95288;0.14917;,
 4.05064;-6.50069;6.58288;,
 2.19646;-8.95288;3.63108;,
 5.28955;-2.83072;8.55522;,
 0.00930;-2.83072;12.03711;,
 -5.27099;-2.83072;8.55522;,
 -5.27099;-2.83072;8.55522;,
 -7.45814;-2.83072;0.14917;,
 -5.27099;-2.83072;-8.25686;,
 -5.27099;-2.83072;-8.25686;,
 0.00930;-2.83072;-11.73877;,
 5.28955;-2.83072;-8.25686;,
 5.28955;-2.83072;-8.25686;,
 7.47672;-2.83072;0.14917;,
 5.28955;-2.83072;8.55522;,
 5.72461;1.49829;9.24780;,
 0.00930;1.49829;13.01659;,
 0.00930;-2.83072;12.03711;,
 0.00930;-2.83072;12.03711;,
 0.00930;1.49829;13.01659;,
 -5.70604;1.49829;9.24780;,
 -8.07340;1.49829;0.14917;,
 -5.70604;1.49829;-8.94947;,
 0.00930;1.49829;-12.71824;,
 0.00930;-2.83072;-11.73877;,
 0.00930;-2.83072;-11.73877;,
 0.00930;1.49829;-12.71824;,
 5.72461;1.49829;-8.94947;,
 8.09198;1.49829;0.14917;,
 5.28955;5.82731;8.55522;,
 0.00930;5.82731;12.03711;,
 0.00930;5.82731;12.03711;,
 -5.27099;5.82731;8.55522;,
 -7.45814;5.82731;0.14917;,
 -5.27099;5.82731;-8.25686;,
 0.00930;5.82731;-11.73877;,
 0.00930;5.82731;-11.73877;,
 5.28955;5.82731;-8.25686;,
 7.47672;5.82731;0.14917;,
 5.28955;5.82731;8.55522;,
 4.05064;9.49727;6.58288;,
 0.00930;9.49727;9.24780;,
 0.00930;5.82731;12.03711;,
 -4.03206;9.49727;6.58288;,
 -5.27099;5.82731;8.55522;,
 -4.03206;9.49727;6.58288;,
 -5.70604;9.49727;0.14917;,
 -4.03206;9.49727;-6.28455;,
 -5.27099;5.82731;-8.25686;,
 -4.03206;9.49727;-6.28455;,
 0.00930;9.49727;-8.94947;,
 0.00930;5.82731;-11.73877;,
 4.05064;9.49727;-6.28455;,
 5.28955;5.82731;-8.25686;,
 4.05064;9.49727;-6.28455;,
 5.72461;9.49727;0.14917;,
 4.05064;9.49727;6.58288;,
 2.19646;11.94945;3.63108;,
 0.00930;11.94945;5.07332;,
 -2.17787;11.94945;3.63108;,
 -4.03206;9.49727;6.58288;,
 -2.17787;11.94945;3.63108;,
 -3.08383;11.94945;0.14917;,
 -5.70604;9.49727;0.14917;,
 -2.17787;11.94945;-3.33273;,
 -4.03206;9.49727;-6.28455;,
 -2.17787;11.94945;-3.33273;,
 0.00930;11.94945;-4.77497;,
 2.19646;11.94945;-3.33273;,
 4.05064;9.49727;-6.28455;,
 2.19646;11.94945;-3.33273;,
 3.10241;11.94945;0.14917;,
 5.72461;9.49727;0.14917;,
 3.10241;11.94945;0.14917;,
 2.19646;11.94945;3.63108;,
 0.00930;-8.95288;5.07332;,
 0.00930;-9.81399;0.14917;,
 2.19646;-8.95288;3.63108;,
 -2.17787;-8.95288;3.63108;,
 -3.08383;-8.95288;0.14917;,
 -2.17787;-8.95288;-3.33273;,
 0.00930;-8.95288;-4.77497;,
 2.19646;-8.95288;-3.33273;,
 3.10241;-8.95288;0.14917;,
 2.19646;11.94945;3.63108;,
 0.00930;12.81054;0.14917;,
 0.00930;11.94945;5.07332;,
 0.00930;11.94945;-4.77497;;
 
 164;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,1,0;,
 3;16,17,18;,
 3;19,17,16;,
 3;20,17,19;,
 3;21,17,20;,
 3;22,17,21;,
 3;23,17,22;,
 3;24,17,23;,
 3;18,17,24;,
 3;25,26,27;,
 3;27,26,28;,
 3;28,26,29;,
 3;29,26,30;,
 3;30,26,31;,
 3;31,26,32;,
 3;32,26,33;,
 3;33,26,25;,
 4;34,35,36,37;,
 4;37,36,38,39;,
 4;39,38,40,41;,
 4;41,40,42,43;,
 4;43,42,44,45;,
 4;45,44,46,47;,
 4;47,46,48,49;,
 4;49,48,35,34;,
 3;50,51,52;,
 3;53,51,50;,
 3;54,51,53;,
 3;55,51,54;,
 3;56,51,55;,
 3;57,51,56;,
 3;58,51,57;,
 3;52,51,58;,
 3;59,60,61;,
 3;61,60,62;,
 3;62,60,63;,
 3;63,60,64;,
 3;64,60,65;,
 3;65,60,66;,
 3;66,60,67;,
 3;67,60,59;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 3;76,77,78;,
 3;77,79,78;,
 3;79,80,78;,
 3;78,80,81;,
 3;80,82,81;,
 3;82,83,81;,
 3;81,83,84;,
 3;84,83,85;,
 4;86,87,88,89;,
 4;90,91,74,73;,
 4;86,92,93,87;,
 3;94,95,96;,
 3;95,97,96;,
 3;97,98,96;,
 3;96,98,99;,
 4;100,101,102,103;,
 3;104,105,106;,
 3;105,107,106;,
 3;106,107,108;,
 3;108,107,109;,
 4;110,111,112,113;,
 4;111,114,115,112;,
 4;116,117,118,119;,
 4;117,120,121,118;,
 4;122,123,124,125;,
 4;92,111,110,93;,
 4;118,72,75,119;,
 4;122,126,127,123;,
 4;118,121,128,72;,
 4;92,129,114,111;,
 4;93,130,131,87;,
 4;110,132,130,93;,
 4;132,110,113,133;,
 4;124,134,135,125;,
 4;136,116,119,137;,
 4;75,138,137,119;,
 4;74,139,138,75;,
 4;91,140,139,74;,
 4;141,68,71,142;,
 4;87,131,143,88;,
 4;131,144,145,143;,
 4;130,146,144,131;,
 4;132,147,146,130;,
 4;147,132,133,148;,
 4;134,149,150,135;,
 4;151,136,137,152;,
 4;138,153,152,137;,
 4;139,154,153,138;,
 4;140,155,154,139;,
 4;156,157,141,142;,
 4;158,159,160,161;,
 4;161,160,162,163;,
 4;164,165,166,167;,
 4;167,166,168,169;,
 4;170,171,172,173;,
 4;173,172,174,175;,
 4;176,177,178,179;,
 4;179,178,180,181;,
 4;159,182,183,160;,
 4;160,183,184,162;,
 4;165,185,186,166;,
 4;166,186,187,168;,
 4;171,188,189,172;,
 4;172,189,190,174;,
 4;177,191,192,178;,
 4;178,192,193,180;,
 4;193,194,195,196;,
 4;197,198,199,185;,
 4;185,199,200,186;,
 4;186,200,201,187;,
 4;187,201,202,203;,
 4;204,205,206,191;,
 4;191,206,207,192;,
 4;192,207,194,193;,
 4;194,208,209,195;,
 4;198,210,211,199;,
 4;199,211,212,200;,
 4;200,212,213,201;,
 4;201,213,214,202;,
 4;205,215,216,206;,
 4;206,216,217,207;,
 4;207,217,208,194;,
 4;218,219,220,221;,
 4;221,220,222,223;,
 4;211,224,225,212;,
 4;212,225,226,213;,
 4;227,228,229,230;,
 4;230,229,231,232;,
 4;216,233,234,217;,
 4;217,234,235,208;,
 4;219,236,237,220;,
 4;220,237,238,222;,
 4;239,240,241,242;,
 4;242,241,243,244;,
 4;228,245,246,229;,
 4;229,246,247,231;,
 4;248,249,250,251;,
 4;234,252,253,235;,
 3;254,255,256;,
 3;257,255,254;,
 3;258,255,257;,
 3;259,255,258;,
 3;260,255,259;,
 3;261,255,260;,
 3;262,255,261;,
 3;256,255,262;,
 3;263,264,265;,
 3;265,264,240;,
 3;240,264,241;,
 3;241,264,243;,
 3;243,264,266;,
 3;266,264,249;,
 3;249,264,250;,
 3;250,264,263;;
 
 MeshMaterialList {
  1;
  164;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\arms_00.png";
   }
  }
 }
 MeshNormals {
  134;
  0.000000;0.660109;0.751170;,
  0.000000;-0.000000;1.000000;,
  0.000000;-0.660110;0.751169;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.660110;-0.751169;,
  0.000000;-0.000000;-1.000000;,
  0.000000;0.660109;-0.751170;,
  0.000000;1.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;-0.660110;0.751169;,
  0.000000;-0.660110;-0.751169;,
  0.000000;-0.000000;-1.000000;,
  0.000000;0.660109;-0.751170;,
  -0.977646;0.155533;0.141481;,
  0.016478;-0.041059;0.999021;,
  -0.000000;-1.000000;0.000000;,
  0.966441;0.155341;0.204598;,
  -0.995857;0.084012;-0.034807;,
  0.994804;0.087335;-0.052316;,
  -0.000005;0.810087;0.586310;,
  0.000007;0.771923;0.635716;,
  -0.936722;0.006487;-0.350015;,
  -0.439804;-0.025494;-0.897732;,
  0.427583;-0.032191;-0.903403;,
  0.927188;-0.001533;-0.374594;,
  0.000019;0.900703;0.434436;,
  0.000006;0.900704;0.434434;,
  -0.000019;0.900703;0.434436;,
  -0.000009;0.900703;0.434434;,
  -0.981120;0.128883;0.144193;,
  -0.981982;0.147845;-0.117699;,
  -0.879031;0.158395;-0.449683;,
  -0.413809;0.203320;-0.887369;,
  0.414274;0.197645;-0.888433;,
  0.860639;0.154592;-0.485182;,
  0.975920;0.152783;-0.155685;,
  0.968455;0.129041;0.213174;,
  0.019933;0.005636;0.999785;,
  -0.297537;0.063219;0.952615;,
  -0.970183;0.104383;0.218746;,
  -0.951591;0.184912;-0.245526;,
  -0.793493;0.224411;-0.565693;,
  -0.398801;0.308304;-0.863659;,
  0.402134;0.303814;-0.863705;,
  0.761252;0.226089;-0.607765;,
  0.935574;0.190187;-0.297540;,
  0.959366;0.104444;0.262124;,
  0.648137;0.075075;0.757814;,
  0.356435;-0.904748;0.233207;,
  0.000000;-0.940084;0.340943;,
  -0.356434;-0.904748;0.233207;,
  -0.488269;-0.872693;-0.000000;,
  -0.356434;-0.904748;-0.233207;,
  0.000000;-0.940084;-0.340942;,
  0.356435;-0.904748;-0.233207;,
  0.488271;-0.872692;-0.000000;,
  0.619534;-0.661890;0.421995;,
  0.000001;-0.752319;0.658799;,
  -0.619532;-0.661891;0.421995;,
  -0.806126;-0.591744;-0.000000;,
  -0.619533;-0.661891;-0.421995;,
  0.000001;-0.752319;-0.658799;,
  0.619534;-0.661890;-0.421995;,
  0.806127;-0.591743;-0.000000;,
  0.767359;-0.344684;0.540698;,
  0.000001;-0.427545;0.903994;,
  -0.767358;-0.344685;0.540698;,
  -0.957178;-0.289499;-0.000000;,
  -0.767358;-0.344684;-0.540698;,
  0.000001;-0.427544;-0.903995;,
  0.767359;-0.344684;-0.540698;,
  0.957178;-0.289499;-0.000000;,
  0.814099;-0.000000;0.580727;,
  0.550504;-0.000000;0.834832;,
  -0.814098;-0.000000;0.580727;,
  -1.000000;-0.000000;-0.000000;,
  -0.814098;-0.000000;-0.580727;,
  -0.550502;-0.000000;-0.834834;,
  0.814099;-0.000000;-0.580727;,
  1.000000;-0.000000;-0.000000;,
  0.767358;0.344684;0.540697;,
  0.512037;0.367246;0.776498;,
  -0.767358;0.344685;0.540698;,
  -0.957178;0.289500;-0.000000;,
  -0.767358;0.344685;-0.540698;,
  -0.512035;0.367245;-0.776499;,
  0.767359;0.344684;-0.540697;,
  0.957178;0.289500;-0.000000;,
  0.619533;0.661891;0.421994;,
  0.000001;0.752320;0.658798;,
  -0.619531;0.661893;0.421994;,
  -0.806125;0.591746;-0.000000;,
  -0.619532;0.661892;-0.421994;,
  0.000001;0.752320;-0.658798;,
  0.619533;0.661891;-0.421994;,
  0.806126;0.591744;-0.000000;,
  0.356433;0.904749;0.233205;,
  0.000001;0.940085;0.340940;,
  -0.356431;0.904750;0.233206;,
  -0.488266;0.872695;-0.000000;,
  -0.356431;0.904750;-0.233205;,
  0.000001;0.940085;-0.340940;,
  0.356432;0.904749;-0.233205;,
  0.488267;0.872694;-0.000000;,
  0.000001;-1.000000;-0.000000;,
  0.000001;1.000000;-0.000000;,
  -0.951429;0.115095;0.285546;,
  -0.945496;0.166108;0.280082;,
  0.904440;0.146819;0.400539;,
  0.902437;0.109526;0.416667;,
  0.000000;1.000000;0.000000;,
  0.000000;0.377858;0.925864;,
  -0.942881;-0.115152;-0.312595;,
  -0.431594;-0.258033;-0.864376;,
  0.415264;-0.263949;-0.870567;,
  0.936699;-0.123708;-0.327554;,
  0.023345;0.052318;0.998358;,
  -0.954824;0.063775;0.290248;,
  -0.950542;0.106149;0.291896;,
  -0.465627;0.066465;0.882481;,
  -0.930000;0.200031;-0.308363;,
  -0.745702;0.258207;-0.614213;,
  -0.388792;0.412690;-0.823728;,
  0.387159;0.408585;-0.826538;,
  0.704088;0.260754;-0.660505;,
  0.908945;0.205866;-0.362545;,
  0.947025;0.106210;0.303090;,
  0.459775;0.071269;0.885171;,
  -0.550502;-0.000000;0.834834;,
  0.550504;-0.000000;-0.834832;,
  -0.512034;0.367246;0.776499;,
  0.512037;0.367245;-0.776498;;
  164;
  4;0,0,1,1;,
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;0,0,10,10;,
  4;10,10,11,11;,
  4;11,11,3,3;,
  4;3,3,12,12;,
  4;12,12,13,13;,
  4;13,13,14,14;,
  4;14,14,7,7;,
  4;7,7,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;39,16,16,39;,
  4;20,18,38,37;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  4;15,31,108,109;,
  4;110,111,38,18;,
  4;15,19,32,31;,
  3;112,112,112;,
  3;112,112,112;,
  3;112,112,112;,
  3;112,112,112;,
  4;113,21,22,113;,
  3;27,22,28;,
  3;22,21,28;,
  3;28,21,30;,
  3;30,21,29;,
  4;33,23,24,34;,
  4;23,114,115,24;,
  4;35,25,26,36;,
  4;25,116,117,26;,
  4;24,25,35,34;,
  4;19,23,33,32;,
  4;26,20,37,36;,
  4;24,115,116,25;,
  4;26,117,8,20;,
  4;19,9,114,23;,
  4;32,42,41,31;,
  4;33,43,42,32;,
  4;43,33,34,44;,
  4;35,45,44,34;,
  4;45,35,36,46;,
  4;37,47,46,36;,
  4;38,48,47,37;,
  4;111,49,48,38;,
  4;118,39,39,40;,
  4;31,41,119,108;,
  4;41,120,121,40;,
  4;42,122,120,41;,
  4;43,123,122,42;,
  4;123,43,44,124;,
  4;45,125,124,44;,
  4;125,45,46,126;,
  4;47,127,126,46;,
  4;48,128,127,47;,
  4;49,129,128,48;,
  4;121,129,49,40;,
  4;50,58,59,51;,
  4;51,59,60,52;,
  4;52,60,61,53;,
  4;53,61,62,54;,
  4;54,62,63,55;,
  4;55,63,64,56;,
  4;56,64,65,57;,
  4;57,65,58,50;,
  4;58,66,67,59;,
  4;59,67,68,60;,
  4;60,68,69,61;,
  4;61,69,70,62;,
  4;62,70,71,63;,
  4;63,71,72,64;,
  4;64,72,73,65;,
  4;65,73,66,58;,
  4;66,74,75,67;,
  4;67,130,76,68;,
  4;68,76,77,69;,
  4;69,77,78,70;,
  4;70,78,79,71;,
  4;71,131,80,72;,
  4;72,80,81,73;,
  4;73,81,74,66;,
  4;74,82,83,75;,
  4;130,132,84,76;,
  4;76,84,85,77;,
  4;77,85,86,78;,
  4;78,86,87,79;,
  4;131,133,88,80;,
  4;80,88,89,81;,
  4;81,89,82,74;,
  4;82,90,91,83;,
  4;132,91,92,84;,
  4;84,92,93,85;,
  4;85,93,94,86;,
  4;86,94,95,87;,
  4;133,95,96,88;,
  4;88,96,97,89;,
  4;89,97,90,82;,
  4;90,98,99,91;,
  4;91,99,100,92;,
  4;92,100,101,93;,
  4;93,101,102,94;,
  4;94,102,103,95;,
  4;95,103,104,96;,
  4;96,104,105,97;,
  4;97,105,98,90;,
  3;51,106,50;,
  3;52,106,51;,
  3;53,106,52;,
  3;54,106,53;,
  3;55,106,54;,
  3;56,106,55;,
  3;57,106,56;,
  3;50,106,57;,
  3;98,107,99;,
  3;99,107,100;,
  3;100,107,101;,
  3;101,107,102;,
  3;102,107,103;,
  3;103,107,104;,
  3;104,107,105;,
  3;105,107,98;;
 }
 MeshTextureCoords {
  267;
  0.774928;0.705660;,
  0.759611;0.705660;,
  0.759611;0.739661;,
  0.774928;0.739661;,
  0.759611;0.773662;,
  0.774928;0.773662;,
  0.759611;0.787745;,
  0.774928;0.787745;,
  0.759611;0.773662;,
  0.774928;0.773662;,
  0.759611;0.739661;,
  0.774928;0.739661;,
  0.759611;0.705660;,
  0.774928;0.705660;,
  0.759611;0.691576;,
  0.774928;0.691576;,
  0.839753;0.720380;,
  0.811043;0.720380;,
  0.831344;0.700079;,
  0.831344;0.740682;,
  0.811043;0.749091;,
  0.790741;0.740682;,
  0.782332;0.720380;,
  0.790741;0.700079;,
  0.811043;0.691670;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.774592;0.705660;,
  0.759275;0.705660;,
  0.759275;0.739661;,
  0.774592;0.739661;,
  0.759275;0.773662;,
  0.774592;0.773662;,
  0.759275;0.787745;,
  0.774592;0.787745;,
  0.759275;0.773662;,
  0.774592;0.773662;,
  0.759275;0.739661;,
  0.774592;0.739661;,
  0.759275;0.705660;,
  0.774592;0.705660;,
  0.759275;0.691576;,
  0.774592;0.691576;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.831344;0.700079;,
  0.811043;0.720380;,
  0.839753;0.720380;,
  0.831344;0.740682;,
  0.811043;0.749091;,
  0.790741;0.740682;,
  0.782332;0.720380;,
  0.790741;0.700079;,
  0.811043;0.691670;,
  0.638381;0.703224;,
  0.649327;0.652590;,
  0.670677;0.652795;,
  0.681477;0.702452;,
  0.581779;0.652690;,
  0.610857;0.652690;,
  0.609309;0.703477;,
  0.584135;0.703477;,
  0.688452;0.773735;,
  0.688452;0.746450;,
  0.694094;0.781164;,
  0.692525;0.742116;,
  0.700277;0.731165;,
  0.720427;0.781164;,
  0.712964;0.731165;,
  0.721558;0.742798;,
  0.726069;0.773735;,
  0.726069;0.746450;,
  0.610663;0.652690;,
  0.610050;0.702241;,
  0.617427;0.702241;,
  0.618579;0.652690;,
  0.618773;0.652690;,
  0.616164;0.703426;,
  0.581585;0.652690;,
  0.582955;0.702241;,
  0.471460;0.743145;,
  0.471593;0.714067;,
  0.501960;0.743284;,
  0.476204;0.706172;,
  0.497555;0.706270;,
  0.502093;0.714207;,
  0.471744;0.745129;,
  0.505649;0.744721;,
  0.506017;0.775219;,
  0.472111;0.775627;,
  0.475606;0.782770;,
  0.475540;0.777288;,
  0.476780;0.785105;,
  0.506038;0.776921;,
  0.504986;0.784765;,
  0.506104;0.782403;,
  0.570089;0.702063;,
  0.574427;0.652264;,
  0.569563;0.652690;,
  0.556728;0.703834;,
  0.562264;0.616137;,
  0.559915;0.615004;,
  0.556922;0.703834;,
  0.569757;0.652690;,
  0.574621;0.652264;,
  0.570508;0.703249;,
  0.560109;0.615004;,
  0.562458;0.616137;,
  0.517439;0.652690;,
  0.537978;0.652690;,
  0.538141;0.703834;,
  0.517276;0.703834;,
  0.513913;0.615004;,
  0.542121;0.615004;,
  0.567941;0.618782;,
  0.567747;0.618782;,
  0.567503;0.753738;,
  0.608479;0.753738;,
  0.558003;0.753676;,
  0.547113;0.754295;,
  0.538141;0.754295;,
  0.516200;0.754295;,
  0.547308;0.754295;,
  0.559063;0.754090;,
  0.568041;0.754170;,
  0.608346;0.754170;,
  0.619320;0.754152;,
  0.636442;0.753934;,
  0.681906;0.753956;,
  0.619635;0.753738;,
  0.607635;0.781382;,
  0.620820;0.781382;,
  0.559207;0.781382;,
  0.551515;0.781382;,
  0.532079;0.781382;,
  0.538140;0.781382;,
  0.515622;0.781382;,
  0.532273;0.781382;,
  0.552920;0.781382;,
  0.559402;0.781382;,
  0.607830;0.781382;,
  0.621015;0.781382;,
  0.682136;0.781603;,
  0.635401;0.781155;,
  0.398228;0.415985;,
  0.395703;0.417571;,
  0.393424;0.414114;,
  0.396995;0.414114;,
  0.395703;0.410658;,
  0.398228;0.412244;,
  0.363676;0.426796;,
  0.365773;0.424271;,
  0.365773;0.429774;,
  0.363676;0.429774;,
  0.365773;0.435276;,
  0.363676;0.432752;,
  0.383636;0.412244;,
  0.386161;0.410658;,
  0.388440;0.414114;,
  0.384870;0.414114;,
  0.386161;0.417571;,
  0.383636;0.415985;,
  0.379706;0.426796;,
  0.381803;0.424271;,
  0.381803;0.429774;,
  0.379706;0.429774;,
  0.381803;0.435276;,
  0.379706;0.432752;,
  0.394017;0.418630;,
  0.391039;0.414114;,
  0.394017;0.409598;,
  0.368912;0.422584;,
  0.368912;0.429774;,
  0.368912;0.436963;,
  0.387848;0.409598;,
  0.390826;0.414114;,
  0.387848;0.418630;,
  0.384942;0.422584;,
  0.384942;0.429774;,
  0.384942;0.436963;,
  0.388645;0.437556;,
  0.388645;0.440779;,
  0.384942;0.439941;,
  0.368912;0.419606;,
  0.372614;0.418769;,
  0.372614;0.421992;,
  0.372614;0.429774;,
  0.372614;0.437556;,
  0.372614;0.440779;,
  0.368912;0.439941;,
  0.384942;0.419606;,
  0.388645;0.418769;,
  0.388645;0.421992;,
  0.388645;0.429774;,
  0.392347;0.436963;,
  0.392347;0.439941;,
  0.376317;0.419606;,
  0.376317;0.422584;,
  0.376317;0.429774;,
  0.376317;0.436963;,
  0.376317;0.439941;,
  0.392347;0.419606;,
  0.392347;0.422584;,
  0.392347;0.429774;,
  0.380446;0.418630;,
  0.378759;0.417571;,
  0.381038;0.414114;,
  0.383424;0.414114;,
  0.378759;0.410658;,
  0.380446;0.409598;,
  0.379455;0.424271;,
  0.379455;0.429774;,
  0.379455;0.435276;,
  0.371810;0.409598;,
  0.373497;0.410658;,
  0.371217;0.414114;,
  0.368832;0.414114;,
  0.373497;0.417571;,
  0.371810;0.418630;,
  0.395486;0.424271;,
  0.395486;0.429774;,
  0.395486;0.435276;,
  0.376234;0.415985;,
  0.377468;0.414114;,
  0.376234;0.412244;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.376021;0.412244;,
  0.374788;0.414114;,
  0.376021;0.415985;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.397583;0.429774;,
  0.397583;0.432752;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
