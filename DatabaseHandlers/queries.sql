--engine queries

-- select engine values
select startSection, nextSection1, nextSection2,
nextSection3, nextSection4, nextSection5, nextSection6,
nextSection7, nextSection8, nextSection9, nextSection10,
throttle, direction from EnginePaths where engine='';

--update engine paths
update EnginePaths
  set(startSection='', nextSection1='', nextSection2='',
  nextSection3='', nextSection4='', nextSection5='',
  nextSection6='', nextSection7='', nextSection8='',
  nextSection9='', nextSection10='') where engine='';

-- set engine path -- should only be called when
-- adding a new train car to the track
Insert into EnginePaths(engine, startSection, nextSection1,
  nextSection2, nextSection3, nextSection4, nextSection5,
  nextSection6, nextSection7, nextSection8, nextSection8,
  nextSection9, nextSection10, throttle, direction)
  values ('','','','','','','','','','','','','',0,0);



-- Section queries

-- Select Section info, this table should be get only
Select x, y, TrackType, numOfConns, conn1, conn2,
conn3, conn4, trackInfoID
From Section
where node='';


-- TrackInfo queries....may move this
-- table to the Section or EnginePaths

-- Select Track Info
