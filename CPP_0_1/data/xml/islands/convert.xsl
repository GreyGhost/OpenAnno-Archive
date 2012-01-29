<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	version="1.0"
>
	<xsl:output
		method="xml"
		indent="yes"
		encoding="utf-8"
	/>
	<xsl:template match="/island">
		<island cols="{@cols}" rows="{@rows}" clime="{@clime}">
			<xsl:apply-templates/>
		</island>
	</xsl:template>
	<xsl:template match="/island/start_row | /island/start_col | /island/name | /island/trader | /island/pirates | /island/drouth | /island/plague">
		<xsl:copy-of select="."/>
	</xsl:template>
	<xsl:template match="/island/terrain">
		<terrain>
			<xsl:apply-templates/>
		</terrain>
	</xsl:template>
	<xsl:template match="/island/terrain/row">
		<row value="{@value}">
			<xsl:apply-templates/>
		</row>
	</xsl:template>
	<xsl:template match="/island/terrain/row/col">
		<col value="{@value}">
			<tile height="1" angle="180" slope="slope" texture="{.}" type="grassland"/>
		</col>
	</xsl:template>
</xsl:stylesheet>
